#include <stdio.h>
#include <stdlib.h>

#define NO_EDGE -9999

// Include the implementation
#include "maxweight_clique_partition.c"

void print_upper_triangular_matrix(int** weights, int n) {
    printf("Upper triangular matrix:\n");
    for (int i = 0; i < n - 1; i++) {
        printf("Row %d: ", i);
        for (int j = 0; j < n - 1 - i; j++) {
            printf("%d ", weights[i][j]);
        }
        printf("\n");
    }
}

void test_sample_input() {
    printf("Testing sample input from problem description:\n");
    
    int n = 5;  // 5 nodes (0-4)
    int k = 5;  // max clique size
    
    // Allocate upper triangular matrix
    int** weights = (int**)malloc((n - 1) * sizeof(int*));
    
    // Row 0: edges from node 0 to nodes 1,2,3,4
    weights[0] = (int*)malloc(4 * sizeof(int));
    weights[0][0] = 3;      // edge (0,1) = 3
    weights[0][1] = 5;      // edge (0,2) = 5
    weights[0][2] = -9999;  // edge (0,3) = no edge
    weights[0][3] = 1;      // edge (0,4) = 1
    
    // Row 1: edges from node 1 to nodes 2,3,4
    weights[1] = (int*)malloc(3 * sizeof(int));
    weights[1][0] = 4;      // edge (1,2) = 4
    weights[1][1] = -9999;  // edge (1,3) = no edge
    weights[1][2] = 5;      // edge (1,4) = 5
    
    // Row 2: edges from node 2 to nodes 3,4
    weights[2] = (int*)malloc(2 * sizeof(int));
    weights[2][0] = -9999;  // edge (2,3) = no edge
    weights[2][1] = 6;      // edge (2,4) = 6
    
    // Row 3: edges from node 3 to node 4
    weights[3] = (int*)malloc(1 * sizeof(int));
    weights[3][0] = 7;      // edge (3,4) = 7
    
    print_upper_triangular_matrix(weights, n);
    
    // Test edge access
    printf("\nTesting edge access:\n");
    printf("Edge (0,1) = %d (expected 3)\n", safe_get_weight(weights, n, 0, 1));
    printf("Edge (1,0) = %d (expected 3)\n", safe_get_weight(weights, n, 1, 0));
    printf("Edge (0,2) = %d (expected 5)\n", safe_get_weight(weights, n, 0, 2));
    printf("Edge (1,2) = %d (expected 4)\n", safe_get_weight(weights, n, 1, 2));
    printf("Edge (3,4) = %d (expected 7)\n", safe_get_weight(weights, n, 3, 4));
    printf("Edge (0,3) = %d (expected -9999)\n", safe_get_weight(weights, n, 0, 3));
    
    // Run algorithm
    int partition_size;
    int* clique_sizes;
    
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    if (partition) {
        printf("\nPartition found with %d cliques:\n", partition_size);
        
        double total_weight = 0.0;
        for (int i = 0; i < partition_size; i++) {
            printf("Clique %d: ", i);
            for (int j = 0; j < clique_sizes[i]; j++) {
                printf("%d ", partition[i][j]);
            }
            
            // Calculate clique weight
            double clique_weight = 0.0;
            for (int j = 0; j < clique_sizes[i]; j++) {
                for (int k = j + 1; k < clique_sizes[i]; k++) {
                    int u = partition[i][j];
                    int v = partition[i][k];
                    int w = safe_get_weight(weights, n, u, v);
                    if (w != NO_EDGE) {
                        clique_weight += w;
                    }
                }
            }
            
            printf("(weight: %.1f)\n", clique_weight);
            total_weight += clique_weight;
        }
        
        printf("Total weight: %.1f, Average per node: %.6f\n", 
               total_weight, total_weight / n);
        
        // Clean up
        for (int i = 0; i < partition_size; i++) {
            free(partition[i]);
        }
        free(partition);
        free(clique_sizes);
    } else {
        printf("ERROR: Algorithm returned NULL\n");
    }
    
    // Clean up weights
    for (int i = 0; i < n - 1; i++) {
        free(weights[i]);
    }
    free(weights);
}

void test_edge_cases() {
    printf("\n\nTesting edge cases:\n");
    
    // Test 1: Single node
    printf("\nTest 1: Single node\n");
    int n = 1, k = 1;
    int** weights = NULL;  // No edges for single node
    
    int partition_size;
    int* clique_sizes;
    
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    if (partition) {
        printf("Single node partition: size=%d\n", partition_size);
        for (int i = 0; i < partition_size; i++) {
            free(partition[i]);
        }
        free(partition);
        free(clique_sizes);
    }
    
    // Test 2: Two nodes with edge
    printf("\nTest 2: Two nodes with edge\n");
    n = 2; k = 2;
    weights = (int**)malloc(1 * sizeof(int*));
    weights[0] = (int*)malloc(1 * sizeof(int));
    weights[0][0] = 10;
    
    partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    if (partition) {
        printf("Two node partition: size=%d\n", partition_size);
        for (int i = 0; i < partition_size; i++) {
            printf("Clique %d:", i);
            for (int j = 0; j < clique_sizes[i]; j++) {
                printf(" %d", partition[i][j]);
            }
            printf("\n");
        }
        
        for (int i = 0; i < partition_size; i++) {
            free(partition[i]);
        }
        free(partition);
        free(clique_sizes);
    }
    
    free(weights[0]);
    free(weights);
}

int main() {
    test_sample_input();
    test_edge_cases();
    
    printf("\nAll tests completed.\n");
    return 0;
}
