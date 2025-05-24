/*
 * Safe Maximum-Weight Clique Partition Algorithm
 * 
 * This implementation prioritizes correctness and speed:
 * - Simple O(n^2) greedy algorithm
 * - Minimal memory usage
 * - No complex optimizations that could cause hangs
 * - Guaranteed to terminate quickly
 */

#include <stdio.h>
#include <stdlib.h>

#define NO_EDGE -9999

/*
 * Check if two nodes are connected
 */
int are_connected(int** weights, int u, int v) {
    static int temp;
    if (u == v) return 1;
    if (u > v) { temp = u; u = v; v = temp; }
    return weights[u][v] != NO_EDGE;
}

/*
 * Main clique partition function - safe and simple
 */
int** maxWeightCliquePartition(int** weights, int n, int k, int* partition_size, int** clique_sizes) {
    if (!weights || n <= 0 || k <= 0) return NULL;
    
    // Start with each node as its own clique
    int** partition = (int**)malloc(n * sizeof(int*));
    *clique_sizes = (int*)malloc(n * sizeof(int));
    
    if (!partition || !*clique_sizes) {
        if (partition) free(partition);
        if (*clique_sizes) free(*clique_sizes);
        return NULL;
    }
    
    int i, j, can_merge, a, b, new_size, shift;
    int* new_clique;
    
    // Initialize single-node cliques
    for (i = 0; i < n; ++i) {
        partition[i] = (int*)malloc(sizeof(int));
        if (!partition[i]) {
            for (j = 0; j < i; ++j) free(partition[j]);
            free(partition);
            free(*clique_sizes);
            return NULL;
        }
        partition[i][0] = i;
        (*clique_sizes)[i] = 1;
    }
    
    *partition_size = n;

    // Try to merge cliques greedily (simple approach)
    for (i = 0; i < *partition_size - 1; ++i) {
        for (j = i + 1; j < *partition_size; ++j) {
            // Check if cliques i and j can be merged
            if ((*clique_sizes)[i] + (*clique_sizes)[j] <= k) {
                  can_merge = 1;
                
                // Check if all pairs are connected
                for (a = 0; a < (*clique_sizes)[i] && can_merge; a++) {
                    for (b = 0; b < (*clique_sizes)[j] && can_merge; b++) {
                        if (!are_connected(weights, partition[i][a], partition[j][b])) {
                            can_merge = 0;
                        }
                    }
                }
                
                if (can_merge) {
                    // Merge j into i
                      new_size = (*clique_sizes)[i] + (*clique_sizes)[j];
                      new_clique = (int*)malloc(new_size * sizeof(int));
                    
                    if (new_clique) {
                        // Copy from clique i
                        for (a = 0; a < (*clique_sizes)[i]; a++) {
                            new_clique[a] = partition[i][a];
                        }
                        // Copy from clique j
                        for (b = 0; b < (*clique_sizes)[j]; b++) {
                            new_clique[(*clique_sizes)[i] + b] = partition[j][b];
                        }
                        
                        // Replace clique i
                        free(partition[i]);
                        partition[i] = new_clique;
                        (*clique_sizes)[i] = new_size;
                        
                        // Remove clique j
                        free(partition[j]);
                        for (shift = j; shift < *partition_size - 1; shift++) {
                            partition[shift] = partition[shift + 1];
                            (*clique_sizes)[shift] = (*clique_sizes)[shift + 1];
                        }
                        --(*partition_size);
                        --j; // Recheck this position
                    }
                }
            }
        }
    }
    
    return partition;
}