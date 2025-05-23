#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_EDGE -9999
#define TRUE 1
#define FALSE 0

// Include the main implementation
#include "maxweight_clique_partition.c"

void test_sample_case() {
    printf("=== Testing Sample Input Case ===\n");
    
    // Sample input: n=5, k=4
    // Input format:
    // 3   5   -9999   1
    // 4   -9999   5
    // -9999   6
    // 7
    
    int n = 5, k = 4;
    int** weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    // Parse the sample input
    // Line 0: 3 5 -9999 1 -> edges (0,1)=3, (0,2)=5, (0,3)=-9999, (0,4)=1
    weights[0][1] = weights[1][0] = 3;
    weights[0][2] = weights[2][0] = 5;
    weights[0][4] = weights[4][0] = 1;
    
    // Line 1: 4 -9999 5 -> edges (1,2)=4, (1,3)=-9999, (1,4)=5
    weights[1][2] = weights[2][1] = 4;
    weights[1][4] = weights[4][1] = 5;
    
    // Line 2: -9999 6 -> edges (2,3)=-9999, (2,4)=6
    weights[2][4] = weights[4][2] = 6;
    
    // Line 3: 7 -> edge (3,4)=7
    weights[3][4] = weights[4][3] = 7;
    
    printf("Graph edges:\n");
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (weights[i][j] != NO_EDGE) {
                printf("Edge (%d,%d) = %d\n", i, j, weights[i][j]);
            }
        }
    }
    
    int partition_size;
    int* clique_sizes;
    
    printf("\nRunning algorithm...\n");
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    printf("\nSolution found:\n");
    printf("Number of cliques: %d\n", partition_size);
    
    double total_weight = 0.0;
    for (int i = 0; i < partition_size; i++) {
        printf("Clique %d (size %d): ", i, clique_sizes[i]);
        for (int j = 0; j < clique_sizes[i]; j++) {
            printf("%d ", partition[i][j]);
        }
        
        // Calculate clique weight
        double clique_weight = 0.0;
        for (int j = 0; j < clique_sizes[i]; j++) {
            for (int k = j + 1; k < clique_sizes[i]; k++) {
                int u = partition[i][j];
                int v = partition[i][k];
                if (u > v) { int temp = u; u = v; v = temp; }
                if (weights[u][v] != NO_EDGE) {
                    clique_weight += weights[u][v];
                }
            }
        }
        
        printf("(weight: %.2f)\n", clique_weight);
        total_weight += clique_weight;
    }
    
    printf("Total weight: %.2f\n", total_weight);
    printf("Average weight: %.6f\n", total_weight / n);
    
    // Validate the solution
    printf("\nValidation:\n");
    
    // Check that all nodes are assigned
    int* node_count = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < partition_size; i++) {
        for (int j = 0; j < clique_sizes[i]; j++) {
            node_count[partition[i][j]]++;
        }
    }
    
    int all_assigned = TRUE;
    for (int i = 0; i < n; i++) {
        if (node_count[i] != 1) {
            printf("Error: Node %d assigned %d times\n", i, node_count[i]);
            all_assigned = FALSE;
        }
    }
    if (all_assigned) {
        printf("✓ All nodes assigned exactly once\n");
    }
    
    // Check clique property
    int valid_cliques = TRUE;
    for (int i = 0; i < partition_size; i++) {
        for (int j = 0; j < clique_sizes[i]; j++) {
            for (int k = j + 1; k < clique_sizes[i]; k++) {
                int u = partition[i][j];
                int v = partition[i][k];
                if (u > v) { int temp = u; u = v; v = temp; }
                if (weights[u][v] == NO_EDGE) {
                    printf("Error: Nodes %d and %d in clique %d are not connected\n", u, v, i);
                    valid_cliques = FALSE;
                }
            }
        }
    }
    if (valid_cliques) {
        printf("✓ All cliques form complete subgraphs\n");
    }
    
    // Check edge coverage
    int edges_covered = 0;
    int total_edges = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (weights[i][j] != NO_EDGE) {
                total_edges++;
                // Check if this edge is covered by some clique
                int covered = FALSE;
                for (int c = 0; c < partition_size && !covered; c++) {
                    int i_in_clique = FALSE, j_in_clique = FALSE;
                    for (int m = 0; m < clique_sizes[c]; m++) {
                        if (partition[c][m] == i) i_in_clique = TRUE;
                        if (partition[c][m] == j) j_in_clique = TRUE;
                    }
                    if (i_in_clique && j_in_clique) {
                        covered = TRUE;
                        edges_covered++;
                    }
                }
                if (!covered) {
                    printf("Error: Edge (%d,%d) not covered by any clique\n", i, j);
                }
            }
        }
    }
    printf("Edge coverage: %d/%d edges covered\n", edges_covered, total_edges);
    if (edges_covered == total_edges) {
        printf("✓ All edges covered\n");
    }
    
    // Clean up
    for (int i = 0; i < partition_size; i++) {
        free(partition[i]);
    }
    free(partition);
    free(clique_sizes);
    free(node_count);
    
    for (int i = 0; i < n; i++) {
        free(weights[i]);
    }
    free(weights);
    
    printf("\n=== Test Complete ===\n\n");
}

void test_simple_case() {
    printf("=== Testing Simple Triangle Case ===\n");
    
    // Simple triangle: 3 nodes, all connected
    int n = 3, k = 3;
    int** weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    weights[0][1] = weights[1][0] = 1;
    weights[0][2] = weights[2][0] = 2;
    weights[1][2] = weights[2][1] = 3;
    
    printf("Triangle with edges: (0,1)=1, (0,2)=2, (1,2)=3\n");
    
    int partition_size;
    int* clique_sizes;
    
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    printf("Solution: %d cliques\n", partition_size);
    for (int i = 0; i < partition_size; i++) {
        printf("Clique %d: ", i);
        for (int j = 0; j < clique_sizes[i]; j++) {
            printf("%d ", partition[i][j]);
        }
        printf("\n");
    }
    
    // Clean up
    for (int i = 0; i < partition_size; i++) {
        free(partition[i]);
    }
    free(partition);
    free(clique_sizes);
    
    for (int i = 0; i < n; i++) {
        free(weights[i]);
    }
    free(weights);
    
    printf("\n=== Simple Test Complete ===\n\n");
}

int main() {
    printf("Maximum-Weight Clique Partition - Test Suite\n");
    printf("============================================\n\n");
    
    test_simple_case();
    test_sample_case();
    
    return 0;
}
