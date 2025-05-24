#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_EDGE -9999
#define MIN_WEIGHT -1000000

typedef struct {
    int u, v;
    int weight;
} Edge;

/*
 * Safe access to upper-triangular matrix
 */
int safe_get_weight(int** weights, int n, int u, int v) {
    // Extensive bounds checking
    if (weights == NULL || n <= 0) return NO_EDGE;
    if (u < 0 || v < 0 || u >= n || v >= n) return NO_EDGE;
    if (u == v) return 0;
    
    // Ensure u < v for upper triangular access
    if (u > v) {
        int temp = u;
        u = v;
        v = temp;
    }
    
    // Check bounds for upper triangular matrix
    if (u >= n - 1) return NO_EDGE;
    
    // Check row exists
    if (weights[u] == NULL) return NO_EDGE;
    
    // Calculate column index
    int col = v - u - 1;
    if (col < 0 || col >= n - 1 - u) return NO_EDGE;
    
    return weights[u][col];
}

/*
 * Check if two nodes are connected
 */
int are_connected(int** weights, int n, int u, int v) {
    if (u == v) return 1;
    return safe_get_weight(weights, n, u, v) != NO_EDGE;
}

/*
 * Check if adding a node to a clique maintains clique property
 */
int can_add_to_clique(int** weights, int n, int* clique, int size, int node) {
    if (clique == NULL || size < 0 || node < 0 || node >= n) return 0;
    
    for (int i = 0; i < size; i++) {
        if (clique[i] < 0 || clique[i] >= n) return 0;
        if (!are_connected(weights, n, clique[i], node)) {
            return 0;
        }
    }
    return 1;
}

/*
 * Compare function for sorting edges by weight (descending)
 */
int compare_edges(const void* a, const void* b) {
    if (a == NULL || b == NULL) return 0;
    Edge* ea = (Edge*)a;
    Edge* eb = (Edge*)b;
    if (eb->weight > ea->weight) return 1;
    if (eb->weight < ea->weight) return -1;
    return 0;
}

/*
 * Main clique partition function
 */
int** maxWeightCliquePartition(int** weights, int n, int k, int* partition_size, int** clique_sizes) {
    // Input validation
    if (n <= 0 || n > 10000 || k <= 0 || k > n) return NULL;
    if (partition_size == NULL || clique_sizes == NULL) return NULL;
    
    // Initialize output
    *partition_size = 0;
    *clique_sizes = NULL;
    
    // Allocate for worst case (each node in its own clique)
    int** partition = (int**)calloc(n, sizeof(int*));
    if (!partition) return NULL;
    
    *clique_sizes = (int*)calloc(n, sizeof(int));
    if (!*clique_sizes) {
        free(partition);
        return NULL;
    }
    
    int* node_assigned = (int*)calloc(n, sizeof(int));
    if (!node_assigned) {
        free(partition);
        free(*clique_sizes);
        *clique_sizes = NULL;
        return NULL;
    }
    
    // Handle single node case
    if (n == 1) {
        partition[0] = (int*)calloc(1, sizeof(int));
        if (partition[0]) {
            partition[0][0] = 0;
            (*clique_sizes)[0] = 1;
            *partition_size = 1;
        }
        free(node_assigned);
        return partition;
    }
    
    // Count and collect edges
    long long edge_capacity = ((long long)n * (n - 1)) / 2;
    if (edge_capacity > 1000000) edge_capacity = 1000000; // Limit edge count
    
    Edge* edges = (Edge*)malloc(edge_capacity * sizeof(Edge));
    if (!edges) {
        // Continue without edge optimization
        for (int i = 0; i < n; i++) {
            partition[i] = (int*)calloc(1, sizeof(int));
            if (partition[i]) {
                partition[i][0] = i;
                (*clique_sizes)[i] = 1;
                (*partition_size)++;
            }
        }
        free(node_assigned);
        return partition;
    }
    
    int edge_count = 0;
    if (weights != NULL) {
        for (int i = 0; i < n - 1 && i < 1000; i++) { // Limit iterations
            if (weights[i] == NULL) continue;
            for (int j = 0; j < n - 1 - i && j < 1000; j++) { // Limit iterations
                int v = i + j + 1;
                if (v >= n) break;
                
                int w = weights[i][j];
                if (w != NO_EDGE && w > MIN_WEIGHT && w < -MIN_WEIGHT) {
                    if (edge_count < edge_capacity) {
                        edges[edge_count].u = i;
                        edges[edge_count].v = v;
                        edges[edge_count].weight = w;
                        edge_count++;
                    }
                }
            }
        }
    }
    
    // Sort edges by weight (descending) if there are edges
    if (edge_count > 0) {
        qsort(edges, edge_count, sizeof(Edge), compare_edges);
    }
    
    // Build initial cliques from high-weight edges
    for (int e = 0; e < edge_count && *partition_size < n; e++) {
        int u = edges[e].u;
        int v = edges[e].v;
        
        if (u < 0 || u >= n || v < 0 || v >= n) continue;
        if (node_assigned[u] || node_assigned[v]) continue;
        
        // Allocate new clique
        partition[*partition_size] = (int*)calloc(k, sizeof(int));
        if (!partition[*partition_size]) continue;
        
        partition[*partition_size][0] = u;
        partition[*partition_size][1] = v;
        (*clique_sizes)[*partition_size] = 2;
        node_assigned[u] = 1;
        node_assigned[v] = 1;
        
        // Try to expand clique
        for (int node = 0; node < n && (*clique_sizes)[*partition_size] < k; node++) {
            if (!node_assigned[node] && 
                can_add_to_clique(weights, n, partition[*partition_size], 
                                (*clique_sizes)[*partition_size], node)) {
                
                // Calculate weight gain
                long long gain = 0;
                for (int i = 0; i < (*clique_sizes)[*partition_size]; i++) {
                    int w = safe_get_weight(weights, n, partition[*partition_size][i], node);
                    if (w != NO_EDGE && w > MIN_WEIGHT && w < -MIN_WEIGHT) {
                        gain += w;
                    }
                }
                
                if (gain > 0) {
                    partition[*partition_size][(*clique_sizes)[*partition_size]] = node;
                    (*clique_sizes)[*partition_size]++;
                    node_assigned[node] = 1;
                }
            }
        }
        
        (*partition_size)++;
    }
    
    // Assign remaining nodes
    for (int node = 0; node < n; node++) {
        if (!node_assigned[node]) {
            int best_clique = -1;
            long long best_gain = MIN_WEIGHT;
            
            // Try existing cliques
            for (int c = 0; c < *partition_size && c < n; c++) {
                if ((*clique_sizes)[c] < k && 
                    can_add_to_clique(weights, n, partition[c], (*clique_sizes)[c], node)) {
                    
                    long long gain = 0;
                    for (int i = 0; i < (*clique_sizes)[c]; i++) {
                        int w = safe_get_weight(weights, n, partition[c][i], node);
                        if (w != NO_EDGE && w > MIN_WEIGHT && w < -MIN_WEIGHT) {
                            gain += w;
                        }
                    }
                    
                    if (gain > best_gain) {
                        best_gain = gain;
                        best_clique = c;
                    }
                }
            }
            
            if (best_clique >= 0 && best_clique < *partition_size) {
                partition[best_clique][(*clique_sizes)[best_clique]] = node;
                (*clique_sizes)[best_clique]++;
            } else {
                // Create new single-node clique
                if (*partition_size < n) {
                    partition[*partition_size] = (int*)calloc(1, sizeof(int));
                    if (partition[*partition_size]) {
                        partition[*partition_size][0] = node;
                        (*clique_sizes)[*partition_size] = 1;
                        (*partition_size)++;
                    }
                }
            }
        }
    }
    
    // Simple merge phase with limited iterations
    int improved = 1;
    int iterations = 0;
    while (improved && iterations < 20) { // Reduced iterations
        improved = 0;
        iterations++;
        
        for (int i = 0; i < *partition_size - 1 && !improved && i < 100; i++) {
            for (int j = i + 1; j < *partition_size && !improved && j < 100; j++) {
                if ((*clique_sizes)[i] + (*clique_sizes)[j] <= k) {
                    // Check if can merge
                    int can_merge = 1;
                    for (int a = 0; a < (*clique_sizes)[i] && can_merge; a++) {
                        for (int b = 0; b < (*clique_sizes)[j] && can_merge; b++) {
                            if (!are_connected(weights, n, partition[i][a], partition[j][b])) {
                                can_merge = 0;
                            }
                        }
                    }
                    
                    if (can_merge) {
                        // Calculate benefit
                        long long benefit = 0;
                        for (int a = 0; a < (*clique_sizes)[i]; a++) {
                            for (int b = 0; b < (*clique_sizes)[j]; b++) {
                                int w = safe_get_weight(weights, n, partition[i][a], partition[j][b]);
                                if (w != NO_EDGE && w > MIN_WEIGHT && w < -MIN_WEIGHT) {
                                    benefit += w;
                                }
                            }
                        }
                        
                        if (benefit > 0) {
                            // Merge j into i
                            int new_size = (*clique_sizes)[i] + (*clique_sizes)[j];
                            int* new_clique = (int*)calloc(new_size, sizeof(int));
                            
                            if (new_clique) {
                                memcpy(new_clique, partition[i], (*clique_sizes)[i] * sizeof(int));
                                memcpy(new_clique + (*clique_sizes)[i], partition[j], (*clique_sizes)[j] * sizeof(int));
                                
                                free(partition[i]);
                                partition[i] = new_clique;
                                (*clique_sizes)[i] = new_size;
                                
                                free(partition[j]);
                                
                                // Shift remaining cliques
                                for (int shift = j; shift < *partition_size - 1; shift++) {
                                    partition[shift] = partition[shift + 1];
                                    (*clique_sizes)[shift] = (*clique_sizes)[shift + 1];
                                }
                                partition[*partition_size - 1] = NULL;
                                (*partition_size)--;
                                improved = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    
    free(edges);
    free(node_assigned);
    
    return partition;
}
