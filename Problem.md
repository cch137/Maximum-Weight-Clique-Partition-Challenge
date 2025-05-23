Please conduct an in-depth and detailed investigation at a PhD-level of expertise. You are required to research and develop the best, most optimal, most powerful, fastest, and most superior algorithmic solution to address this problem. Document your research findings in a LaTeX report.

## Maximum-Weight Clique Partition Challenge

### Description

You are given an undirected graph $G = (V, E)$ on $n$ nodes (indexed $1$ through $n$), where each edge $(i, j)$ carries a real weight $w(i, j)$ (which can be positive or negative).

Your task is to divide the nodes into disjoint clusters, called "cliques," such that:

1.  **Adjacency Pair:** Every pair of nodes in the same clique must be connected by an edge in the input.
2.  **No Isolated Node:** Each node belongs to exactly one clique.
3.  **No Missing Edges:** Every input edge must be part of at least one clique.
4.  **Clique Size:** Each clique must consist of at least $1$ node and at most $k \le n$ nodes.

Your objective is to "maximize the average value of the edge weights within the cliques."

### Final Value Calculation

The final value will be calculated as:

$$\text{Weight} = \frac{\sum_{i=1}^M \sum_{(u, v) \in C_i} w(u, v)}{\sum_{i=1}^M |C_i|}$$

where:

- $M$ is the total number of cliques.
- $C_i$ represents the $i$-th clique.
- $(u, v) \in C_i$ means that $u$ and $v$ are nodes within clique $C_i$ and are connected by an edge.
- $w(u, v)$ is the weight of the edge between nodes $u$ and $v$.
- $|C_i|$ represents the number of nodes in clique $C_i$.

### Input

An undirected graph with $n$ nodes is given in upper-triangular format.

- The input consists of $n - 1$ lines.
- The $i$-th line (0-indexed) contains $n - 1 - i$ space-separated integers, representing the edge weights from node $i$ to nodes $i + 1$ through $n - 1$.
- The graph is symmetric: weight between nodes $u$ and $v$ is the same as between $v$ and $u$.
- A value of $-9999$ represents no edge between nodes.

**Example:**

If the first line has $4$ values: `2 3 5 -9999`, this corresponds to:

- edge $(0, 1)$ with weight $2$
- edge $(0, 2)$ with weight $3$
- edge $(0, 3)$ with weight $5$
- edge $(0, 4)$ with weight $-9999$ (no edge)
- edge $(1, 2)$ with weight $2$

### Output

Return a 2D array of integers representing the optimal partition of nodes into cliques.

- Each inner list represents a clique.
- Each node index is an integer in the range $[0, n - 1]$, where $n$ is the total number of nodes.

**Sample Input 1:**

```
3   5   -9999   1
4   -9999   5
-9999   6
7
```

**Sample Output 1:**

```
1 2 4
```

### Answer Here

```C
/*
 *  partition_size: Size of your partition, i.e., number of cliques
 *  clique_sizes: Size of each clique in your partition
 */
int** maxWeightCliquePartition(int** weights, int n, int k, int* partition_size, int** clique_sizes) {
    //Your code here
}
```
