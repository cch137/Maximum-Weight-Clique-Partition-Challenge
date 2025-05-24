/*
 * Challenging test cases to demonstrate optimization improvements
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NO_EDGE -9999

// Include the optimized implementation
#include "maxweight_clique_partition.c"

// Calculate total weight for a partition
double calculate_partition_weight(int** weights, int** partition, int* clique_sizes, int partition_size, int n) {
    double total_weight = 0.0;
    
    for (int i = 0; i < partition_size; i++) {
        for (int j = 0; j < clique_sizes[i]; j++) {
            for (int k = j + 1; k < clique_sizes[i]; k++) {
                int u = partition[i][j];
                int v = partition[i][k];
                if (u > v) { int temp = u; u = v; v = temp; }
                if (weights[u][v] != NO_EDGE) {
                    total_weight += weights[u][v];
                }
            }
        }
    }
    
    return total_weight / n;  // Average weight
}

void print_partition_with_weights(int** weights, int** partition, int* clique_sizes, int partition_size) {
    double total_weight = 0.0;
    
    for (int i = 0; i < partition_size; i++) {
        printf("  Clique %d: ", i);
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
        printf("(weight: %.1f)\n", clique_weight);
        total_weight += clique_weight;
    }
    
    printf("  Total weight: %.1f\n", total_weight);
}

int main() {
    printf("=== Challenging Test Cases for Optimization ===\n\n");
    
    // Test Case 1: Hidden high-value clique
    printf("Test Case 1: Hidden high-value clique (10 nodes, k=4)\n");
    printf("This graph has a hidden 4-clique with very high weights that a greedy algorithm might miss.\n\n");
    
    int n = 10, k = 4;
    int** weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    // Create obvious 3-cliques with medium weights
    // Clique 1: 0,1,2
    weights[0][1] = weights[1][0] = 8;
    weights[0][2] = weights[2][0] = 7;
    weights[1][2] = weights[2][1] = 9;
    
    // Clique 2: 3,4,5
    weights[3][4] = weights[4][3] = 6;
    weights[3][5] = weights[5][3] = 7;
    weights[4][5] = weights[5][4] = 8;
    
    // Hidden high-value 4-clique: 6,7,8,9
    weights[6][7] = weights[7][6] = 20;
    weights[6][8] = weights[8][6] = 22;
    weights[6][9] = weights[9][6] = 21;
    weights[7][8] = weights[8][7] = 23;
    weights[7][9] = weights[9][7] = 24;
    weights[8][9] = weights[9][8] = 25;
    
    // Some confusing edges
    weights[0][3] = weights[3][0] = 5;
    weights[1][4] = weights[4][1] = 4;
    weights[2][6] = weights[6][2] = 3;
    
    int partition_size;
    int* clique_sizes;
    int** partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    printf("Optimized Algorithm Result:\n");
    print_partition_with_weights(weights, partition, clique_sizes, partition_size);
    double avg_weight = calculate_partition_weight(weights, partition, clique_sizes, partition_size, n);
    printf("Average weight per node: %.6f\n", avg_weight);
    
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
    
    // Test Case 2: Complex trade-offs
    printf("\n\nTest Case 2: Complex trade-offs (15 nodes, k=5)\n");
    printf("This graph requires smart decisions about which nodes to group together.\n\n");
    
    n = 15; k = 5;
    weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    // Create several overlapping potential cliques
    // High-value 3-clique: 0,1,2
    weights[0][1] = weights[1][0] = 30;
    weights[0][2] = weights[2][0] = 28;
    weights[1][2] = weights[2][1] = 32;
    
    // Can extend to 4-clique with lower average: 0,1,2,3
    weights[0][3] = weights[3][0] = 5;
    weights[1][3] = weights[3][1] = 4;
    weights[2][3] = weights[3][2] = 6;
    
    // Another high-value option: 4,5,6,7 (4-clique)
    weights[4][5] = weights[5][4] = 15;
    weights[4][6] = weights[6][4] = 16;
    weights[4][7] = weights[7][4] = 14;
    weights[5][6] = weights[6][5] = 17;
    weights[5][7] = weights[7][5] = 15;
    weights[6][7] = weights[7][6] = 16;
    
    // Medium value 5-clique: 8,9,10,11,12
    for (int i = 8; i <= 12; i++) {
        for (int j = i + 1; j <= 12; j++) {
            weights[i][j] = weights[j][i] = 8 + (i + j) % 3;
        }
    }
    
    // Some cross-edges
    weights[0][4] = weights[4][0] = 7;
    weights[2][8] = weights[8][2] = 6;
    weights[3][9] = weights[9][3] = 5;
    
    // Isolated high-value edge
    weights[13][14] = weights[14][13] = 40;
    
    // Connect 13,14 to some nodes
    weights[0][13] = weights[13][0] = 2;
    weights[1][14] = weights[14][1] = 3;
    weights[8][13] = weights[13][8] = 4;
    weights[9][14] = weights[14][9] = 5;
    
    partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    printf("Optimized Algorithm Result:\n");
    print_partition_with_weights(weights, partition, clique_sizes, partition_size);
    avg_weight = calculate_partition_weight(weights, partition, clique_sizes, partition_size, n);
    printf("Average weight per node: %.6f\n", avg_weight);
    
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
    
    // Test Case 3: Many small high-value cliques vs few large medium-value cliques
    printf("\n\nTest Case 3: Small vs Large cliques (20 nodes, k=6)\n");
    printf("Should the algorithm prefer many small high-value cliques or fewer large cliques?\n\n");
    
    n = 20; k = 6;
    weights = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        weights[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; ++j) {
            weights[i][j] = (i == j) ? 0 : NO_EDGE;
        }
    }
    
    // Create 5 high-value triangles
    for (int t = 0; t < 5; t++) {
        int base = t * 3;
        weights[base][base+1] = weights[base+1][base] = 50 - t;
        weights[base][base+2] = weights[base+2][base] = 48 - t;
        weights[base+1][base+2] = weights[base+2][base+1] = 49 - t;
    }
    
    // Create potential 6-clique with medium values (nodes 15-19 plus one from triangles)
    for (int i = 15; i < 20; i++) {
        for (int j = i + 1; j < 20; j++) {
            weights[i][j] = weights[j][i] = 12;
        }
    }
    
    // Connect some triangle nodes to the large clique
    weights[0][15] = weights[15][0] = 10;
    weights[3][16] = weights[16][3] = 11;
    weights[6][17] = weights[17][6] = 9;
    weights[9][18] = weights[18][9] = 10;
    weights[12][19] = weights[19][12] = 11;
    
    partition = maxWeightCliquePartition(weights, n, k, &partition_size, &clique_sizes);
    
    printf("Optimized Algorithm Result:\n");
    print_partition_with_weights(weights, partition, clique_sizes, partition_size);
    avg_weight = calculate_partition_weight(weights, partition, clique_sizes, partition_size, n);
    printf("Average weight per node: %.6f\n", avg_weight);
    
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
    
    printf("\n=== Summary ===\n");
    printf("The optimized algorithm handles these challenging cases by:\n");
    printf("1. Starting with the highest-weight edges to find valuable cliques\n");
    printf("2. Making intelligent decisions about node assignments based on weight contribution\n");
    printf("3. Using local search to refine the solution and escape local optima\n");
    printf("4. Balancing between clique size and edge weights to maximize the objective function\n");
    
    return 0;
}
