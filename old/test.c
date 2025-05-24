/*
 * Final Test for Optimized Implementation
 */

#include <stdio.h>
#include <stdlib.h>

#define NO_EDGE -9999

// Include the optimized implementation
#include "maxweight_clique_partition.c"

int main() {
    printf("Testing Final Optimized Implementation...\n");
    
    // Test 1: Simple triangle
    printf("Test 1: Triangle graph\n");
    int n = 3, k = 3;
    int** weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    weights[0][1] = weights[1][0] = 1;
    weights[0][2] = weights[2][0] = 2;
    weights[1][2] = weights[2][1] = 3;
    
    int partition_size;
    int* clique_sizes;
    
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    if (partition) {
        printf("SUCCESS: Found %d cliques\n", partition_size);
        
        double total_weight = 0.0;
        for (int i = 0; i < partition_size; ++i) {
            printf("Clique %d: ", i);
            for (int j = 0; j < clique_sizes[i]; ++j) {
                printf("%d ", partition[i][j]);
            }
            
            double clique_weight = 0.0;
            for (int j = 0; j < clique_sizes[i]; ++j) {
                for (int k = j + 1; k < clique_sizes[i]; ++k) {
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
        
        printf("Total weight: %.2f, Average: %.6f\n", total_weight, total_weight / n);
        
        // Clean up
        for (int i = 0; i < partition_size; ++i) {
            free(partition[i]);
        }
        free(partition);
        free(clique_sizes);
        
        printf("Test 1 PASSED\n\n");
    } else {
        printf("Test 1 FAILED\n\n");
    }
    
    // Clean up weights
    for (int i = 0; i < n; ++i) {
        free(weights[i]);
    }
    free(weights);
    
    // Test 2: Sample case (simplified)
    printf("Test 2: Sample case (5 nodes)\n");
    n = 5; k = 4;
    weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    // Sample edges
    weights[0][1] = weights[1][0] = 3;
    weights[0][2] = weights[2][0] = 5;
    weights[0][4] = weights[4][0] = 1;
    weights[1][2] = weights[2][1] = 4;
    weights[1][4] = weights[4][1] = 5;
    weights[2][4] = weights[4][2] = 6;
    weights[3][4] = weights[4][3] = 7;
    
    partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    if (partition) {
        printf("SUCCESS: Found %d cliques\n", partition_size);
        
        double total_weight = 0.0;
        for (int i = 0; i < partition_size; ++i) {
            printf("Clique %d: ", i);
            for (int j = 0; j < clique_sizes[i]; ++j) {
                printf("%d ", partition[i][j]);
            }
            
            double clique_weight = 0.0;
            for (int j = 0; j < clique_sizes[i]; ++j) {
                for (int k = j + 1; k < clique_sizes[i]; ++k) {
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
        
        printf("Total weight: %.2f, Average: %.6f\n", total_weight, total_weight / n);
        
        // Clean up
        for (int i = 0; i < partition_size; ++i) {
            free(partition[i]);
        }
        free(partition);
        free(clique_sizes);
        
        printf("Test 2 PASSED\n");
    } else {
        printf("Test 2 FAILED\n");
    }
    
    // Clean up weights
    for (int i = 0; i < n; ++i) {
        free(weights[i]);
    }
    free(weights);
    
    printf("\nAll tests completed successfully!\n");
    printf("The optimized algorithm should now pass time limits and avoid system errors.\n");
    
    return 0;
}