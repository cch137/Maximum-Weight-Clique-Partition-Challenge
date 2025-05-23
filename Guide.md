# Maximum-Weight Clique Partition Problem

We consider an undirected graph \$G=(V,E)\$ with a nonnegative weight \$w\_{ij}\$ on each edge \$(i,j)\in E\$, and wish to partition the vertex set \$V\$ into disjoint subsets (cliques) \$C_1,\dots,C_m\$ so that (1) each \$C\_\ell\$ induces a complete subgraph in \$G\$, (2) each node belongs to exactly one clique, (3) every original edge is contained within some clique, and (4) \$1\le |C\_\ell|\le k\$ for all \$\ell\$. The objective is to **maximize** the average weight of edges inside the cliques, i.e.

$$
\text{AverageWeight} = \frac{\sum_{\ell=1}^m \sum_{\{i,j\}\subset C_\ell} w_{ij}}{\sum_{\ell=1}^m |C_\ell|}\,.
$$

Since \$\sum\_\ell |C\_\ell|=|V|\$ is constant, this is equivalent to maximizing the **total** internal weight \$\sum\_\ell \sum\_{{i,j}\subset C\_\ell} w\_{ij}\$. Equivalently, one can introduce binary variables \$x\_{ij}=1\$ iff vertices \$i,j\$ lie in the same clique, and maximize

$$
\sum_{\{i,j\}\in E} w_{ij}x_{ij},
$$

subject to clique constraints. In particular, the transitivity (triangle) constraints

$$
x_{ij} + x_{jk} - x_{ik} \;\le\; 1,\quad \forall\,i,j,k\in V,
$$

together with \$x\_{ii}=1\$ and clique-size bounds, ensure that the chosen groups are cliques. This is essentially a **clique-partition** or **edge-clique-cover** problem on a complete graph (with missing edges disallowed by the clique condition).

## Complexity and Theoretical Properties

This problem generalizes classical clique covers and is strongly NP-hard in general. In fact, even deciding if a partition into cliques of size at most \$k\$ achieves a given weight is NP-complete. For \$k=1\$ or \$k=2\$ the problem reduces to trivial cases (size-1 cliques or a maximum-weight matching, respectively), but for \$k\ge3\$ it becomes intractable. For example, Oksa and Vajtersič (2006) note that finding a maximum-weight decomposition of a complete graph into cliques of size \$r>2\$ is NP-hard. More generally, the decision version of clique partition (asking if the total weight can reach a threshold) is NP-complete. Feldmann _et al._ (2020) show that even the unweighted _edge-clique-partition_ problem is APX-hard on restricted graph classes, implying there is unlikely to be any polynomial-time approximation scheme in general.

From a parameterized perspective, if the number of cliques \$m\$ is small, fixed-parameter algorithms exist. Feldmann _et al._ give an FPT algorithm for the weighted _edge clique partition_ (cover) parameterized by \$m\$ (and maximum weight). However, our variant fixes the maximum clique size \$k\$ rather than \$m\$, and the number of cliques could be large (up to \$|V|/1\$). The related _balanced \$k\$-way partition_ problem (clique partitioning with equal-size and weight constraints) has been shown NP-hard by reduction from 3-coloring. In particular, Recalde _et al._ (2017) prove NP-hardness of the _balanced clique partition_ via a polynomial transformation. In summary, the Maximum-Weight Clique Partition is strongly NP-hard (no efficient exact algorithm is known) and even hard to approximate in general.

## Integer Programming and Polyhedral Models

A standard exact formulation uses binary variables \$x\_{ij}\$ as above. Equivalently, one can assign each vertex to exactly one clique index; however the \$x\_{ij}\$-model is compact. The objective is

$$
\max\;\sum_{(i,j)\in E} w_{ij}x_{ij},
$$

subject to triangle inequalities (for all triples \$i,j,\ell\$: \$x\_{ij}+x\_{j\ell}-x\_{i\ell}\le1\$) and symmetry (we may fix an ordering to avoid symmetric solutions). The clique-size constraint \$|C|\le k\$ can be imposed by requiring that for each vertex \$i\$, at most \$k-1\$ neighbors \$j\$ have \$x\_{ij}=1\$ (since if \$i\$ is in a clique of size \$M\$, it has \$M-1\$ other members). Such 0–1 LP formulations have a well-studied polytope: Groetschel and Wakabayashi (1989,1990) introduced _clique-partitioning_ polyhedral cuts (triangle and 2-partition inequalities) and showed these describe facets of the partition polytope. Labbé and Özsoy (2010) further examined _size-constrained graph partitioning_ polytopes (which generalize our problem).

In practice, modern solvers implement these constraints via a branch-and-cut. For example, Simançhev _et al._ (2019) construct a cutting-plane algorithm using known facet inequalities, integrated into a branch-and-cut framework for exact solutions. Likewise, Catanzaro _et al._ (2011) implement a branch-and-cut for a related _hub location/clique partition_ problem (with clique-size bounds). Letchford _et al._ (2020) provide a CP-Lib of benchmark instances for the clique partition problem; their own solver is a branch-and-cut on the 0–1 formulation. These exact algorithms can solve moderate-size instances (typically up to a few hundred vertices) to optimality, given enough time, by exploiting LP relaxations and cutting planes.

**Mathematical formulation (ILP).** One can define \$x\_{ij}=1\$ if nodes \$i,j\$ are in the same clique, and 0 otherwise. Then the problem is:

$$
\begin{aligned}
\max\; &\sum_{\{i,j\}\in E} w_{ij}\,x_{ij}\\
\text{s.t. } &x_{ii}=1,\quad\forall i\in V,\\
&x_{ij}+x_{jk}-x_{i\ell} \le 1,\quad \forall i,j,\ell\in V\quad(\text{triangle inequalities}),\\
&x_{ij}\in\{0,1\},\quad \forall i<j.
\end{aligned}
$$

To enforce the size limit \$|C|\le k\$, one may add constraints such as \$\sum\_{j\neq i} x\_{ij} \le k-1\$ for each \$i\$. This ensures no clique exceeds size \$k\$. (The exact enforcement of clique-size constraints often requires additional variables or valid inequalities.) This 0–1 LP can be attacked by branch-and-bound/cut.

## Exact Algorithms and Branch-and-Cut

Given the NP-hardness, exact solvers rely on branch-and-bound or branch-and-cut. The key techniques involve clever branching, bounds and cutting planes. Historically, Groetschel and Wakabayashi’s cutting-plane method (1989–90) solved small clustering instances using polyhedral cuts. More recent works (e.g. Simançhev _et al._ 2019 and Catanzaro _et al._ 2011) combine LP relaxations with branch-and-cut, separating triangle inequalities and other clique-partition facets. Jaehn and Pesch (2013) designed a dedicated branch-and-bound with strong bounds for each node in the search tree. These exact algorithms are guaranteed optimal but scale poorly: typically, only graphs of size \$n\le100\$ can be solved in reasonable time (depending on density and \$k\$). Their performance is highly instance-dependent, and they often serve as subroutines (e.g. to verify small subproblems or in decomposition algorithms).

For very small \$k\$, one can exploit simpler structures. Notably, if \$k=2\$ (only cliques of size at most 2), the problem reduces to finding a maximum-weight _matching_ in the complement of the desired cliques, which is polynomial-time solvable. However, for \$k\ge3\$ (clique sizes up to 3 or more), the problem is NP-hard. There are no known general approximation guarantees for arbitrary graphs: Feldmann _et al._ showed that even the unweighted variant is APX-hard, so one cannot hope for efficient algorithms with provable constant-factor approximation in general.

### Parameterized and Special Cases

In practice, one may exploit structure. For example, if the input graph has certain properties (planar, chordal, interval, etc.), specialized algorithms or approximation schemes might exist. For unit-disk or geometric graphs, PTAS are sometimes possible (by spatial partitioning techniques), although these results are mostly for _minimum_ clique cover rather than _maximum_ weight. In another direction, when the number of cliques \$m\$ is small, fixed-parameter algorithms can be used. Feldmann _et al._ (2020) give an FPT algorithm parameterized by \$m\$ (and by maximum edge weight) for the _edge-clique-partition_ problem. In our context, if one can bound \$m\$ or \$k\$ very small, exponential algorithms with complexity like \$O(n^{k})\$ or \$O^\*(2^{O(m\sqrt{m})})\$ may be feasible. However, since \$m\$ could be as large as \$n\$ when all cliques are small, this is of limited utility unless some problem-specific parameter is small.

## Heuristic and Metaheuristic Methods

Because exact methods are limited to small graphs, heuristics and metaheuristics are widely used for larger instances. These methods sacrifice optimality guarantees for speed and scalability. Common approaches include greedy constructive algorithms, local search, and population-based metaheuristics. One can start with a greedy partition (e.g. repeatedly forming a heavy clique and removing its vertices) and then refine it by local moves (moving a vertex to a different clique, swapping vertices between cliques, or merging/splitting cliques) to increase total weight.

Notably, several advanced heuristics have been proposed:

- **GRASP and Local Search:** Zhou _et al._ (2016) present a sophisticated three-phase local search (called CPP-P3) that iterates descent, tabu-based exploration, and directed perturbations. They define a “Top Move” neighborhood and apply tabu search with GRASP-like perturbations. On benchmark instances this approach found new best solutions, outperforming previous algorithms.

- **Fixed-Set Search:** Jovanović _et al._ (2023) develop a _Fixed-Set Search (FSS)_ metaheuristic, which augments a GRASP procedure with a learning-based fixed-set mechanism. FSS yields very high-quality partitions and “significantly outperforms state-of-the-art metaheuristics” on common benchmarks, obtaining many new best-known solutions. It combines a randomized greedy construction with local search (e.g. simulated annealing in a novel neighborhood) and a scheme to preserve good substructures.

- **Genetic Algorithms:** Oksa and Vajtersič (2006) describe a genetic algorithm for the clique-partition block ordering problem. They encode a partition (decomposition into \$\ell\$ cliques) as a binary string and use specialized crossover and mutation operators that preserve clique validity. The fitness of a genome is the total weight of its cliques. Such GAs (often implemented with libraries like GALIB) can explore the solution space broadly but may require tuning (population size, mutation rate) to be effective.

- **Simulated Annealing and Variants:** Earlier work includes improved simulated annealing and local-search heuristics. For instance, Dorne _et al._ (2005) propose an iterated simulated annealing with special neighborhood moves for the clique partition problem (not shown above, but referenced by heuristics literature), and earlier algorithms such as Kernighan–Lin adaptation, or cluster-editing based methods, have been applied. Heuristics often start from a random or greedy partition and apply local refinements until no improvement is possible.

- **Tabu Search and GRASP Hybrids:** Hybrid strategies combining Tabu Search, GRASP, or Variable Neighborhood Search have been used. For example, Recalde _et al._ (2017) used tabu search both as a primal heuristic and as a basis for perturbation in a branch-and-cut algorithm for sports team realignment (a clique-partition case). These hybrids aim to balance intensification (refining a good solution) and diversification (exploring new partitions).

In summary, heuristic/metaheuristic methods excel on large instances (hundreds to thousands of vertices) and often find near-optimal solutions in reasonable time. They can exploit problem-specific moves (like adding/removing a vertex from a clique, merging small cliques, etc.) and use objective-driven local search to improve iteratively. However, they provide no optimality certificate and their quality varies by instance. Empirical studies (e.g. those by Zhou _et al._ and Jovanović _et al._) show that well-designed heuristics can handle much larger graphs than exact methods, and often reach solutions that are close to or match the best known optima.

## Comparative Summary and Recommendations

- **Exact vs Heuristic:** Exact branch-and-cut algorithms (based on ILP or cutting planes) guarantee optimality and can exploit problem structure, but typically handle only up to \$n=50\$–\$100\$ in practice, depending on graph density and \$k\$. In contrast, heuristic/metaheuristic methods (GRASP, GA, FSS, etc.) can scale to much larger \$n\$ (often hundreds or more) but only yield approximate solutions. When \$n\$ is small or high precision is required, exact B\&C or B\&B with strong bounds is appropriate. For large or real-time problems, one should employ heuristics like those above, possibly combined with partial exact search on subproblems.

- **Algorithm Design:** A practical strategy is to formulate the ILP model and use an advanced solver (Gurobi/CPLEX) with custom callbacks for triangle inequalities; add any problem-specific cuts if known. For larger instances, one might first run a fast heuristic (e.g. greedy or local search) to get a good incumbent, then use that as a starting solution for local improvement or as a primal bound in a B\&C. Parameterized or specialized algorithms can be used if \$k\$ or number of cliques is very small.

- **Recent Advances:** The latest research (2020–2023) focuses on stronger cutting planes and intelligent heuristics. For example, Sørensen (2020) developed specialized cut-separation heuristics to improve the LP relaxation. Jovanović _et al._ (2023) showed that modern metaheuristics (FSS) can effectively exploit structural patterns and symmetry in clique partitions. These suggest that combining learning-based heuristics with polyhedral cutting planes is a promising direction.

In conclusion, the Maximum-Weight Clique Partition problem is a challenging NP-hard optimization task. The **best practice** depends on instance size: for small graphs or when an optimal solution is imperative, use an exact branch-and-cut algorithm with strong polyhedral cuts; for larger graphs, employ advanced heuristics (e.g. the FSS/GRASP approach or tabu-based local search) to obtain high-quality solutions rapidly. A hybrid approach—using heuristics to generate good initial solutions and bounds, combined with exact methods on critical subproblems—often yields the best trade-off between solution quality and computational feasibility.

**References:** Key findings and algorithms above are supported by the literature on clique partitioning, which provides both theoretical analyses and detailed algorithmic descriptions. Notably, Sørensen (2020) and Simançhev _et al._ (2019) discuss polyhedral/branch-and-cut approaches, while Zhou _et al._ (2016) and Jovanović _et al._ (2023) present state-of-the-art metaheuristics. These sources offer in-depth models, complexity proofs, and empirical performance evaluations relevant to the Maximum-Weight Clique Partition problem.

# Key Points

- Research suggests the problem involves partitioning a graph into disjoint cliques with size constraints, maximizing edge weight averages, but the objective value seems constant for valid partitions.
- It seems likely that the task is to find any valid partition where each clique has at most \( k \) nodes and every edge is within a clique, given the graph's structure.
- The evidence leans toward using a greedy approach to iteratively build cliques, ensuring all edges are covered, though feasibility depends on graph connectivity and \( k \).

# Problem Understanding

The Maximum-Weight Clique Partition Challenge requires partitioning an undirected graph
\( G = (V, E) \) with \( n \) nodes into disjoint cliques, each with at most \( k \) nodes, such that every pair in a clique is connected by an edge, no node is isolated, every edge is part of at least one clique, and the average value of edge weights within cliques is maximized. The final value is calculated as the total sum of edge weights within cliques divided by the total number of nodes \( n \).

# Analysis and Approach

Initial analysis suggests that for any valid partition meeting all conditions, the objective value (average edge weight) is constant, as it equals the sum of all edge weights divided by \( n \). This implies the task is to find any valid partition, not necessarily optimize a variable objective. The key challenge is ensuring every edge is within at least one clique, which means for every edge \( (u, v) \), \( u \) and \( v \) must be in the same clique, requiring the partition to respect graph connectivity without edges between different cliques.

Given this, a greedy approach is proposed: iteratively build cliques of size up to \( k \) by starting with high-degree unassigned nodes and adding compatible neighbors, ensuring each clique is a complete subgraph. This process repeats until all nodes are assigned, aiming to cover all edges within the cliques formed.

# Implementation Considerations

The implementation involves parsing the weight matrix, where \(-9999\) indicates no edge, and using dynamic memory allocation in C to manage the partition. The approach ensures cliques are disjoint and of size at most \( k \), but feasibility depends on whether the graph can be partitioned such that no edges exist between different cliques, effectively requiring the graph to be a cluster graph (disjoint union of cliques) for certain \( k \).

# Survey Note: Detailed Investigation of the Maximum-Weight Clique Partition Challenge

## Introduction

This report conducts an in-depth investigation into the Maximum-Weight Clique Partition Challenge, aiming to develop an optimal algorithmic solution at a PhD-level of expertise. The problem involves partitioning an undirected graph \( G = (V, E) \) with \( n \) nodes into disjoint cliques, each with at most \( k \) nodes, ensuring every pair in a clique is connected, no node is isolated, every edge is part of at least one clique, and maximizing the average value of edge weights within cliques. The final value is given by:

\[
\text{Weight} = \frac{\sum*{i=1}^M \sum*{(u, v) \in C*i} w(u, v)}{\sum*{i=1}^M |C_i|}
\]

where \( M \) is the number of cliques, \( C*i \) is the \( i \)-th clique, \( (u, v) \in C_i \) denotes pairs within \( C_i \) connected by an edge, \( w(u, v) \) is the edge weight, and \( |C_i| \) is the size of \( C_i \), with \( \sum*{i=1}^M |C_i| = n \).

## Problem Analysis

The input is provided in upper-triangular format, with edge weights given for \( i < j \), and \(-9999\) indicating no edge. The graph is symmetric, and the output is a 2D array representing the partition into cliques. The sample input for \( n=5 \) and corresponding output "1 2 4" suggest a partition, but detailed analysis is needed to ensure all conditions are met.

## Initial Interpretation

The conditions imply:

- Each clique \( C_i \) must be a complete subgraph (clique) in \( G \).
- The cliques are disjoint and cover all nodes (\( V = \cup C_i \), \( C_i \cap C_j = \emptyset \) for \( i \neq j \)).
- Every edge \( (u, v) \in E \) must be within at least one clique, meaning there exists \( C_i \) such that \( u, v \in C_i \).
- Each clique size is \( 1 \leq |C_i| \leq k \).
- The objective is to maximize the average, which, given the formula, is:

\[
\frac{\text{total sum of edge weights within cliques}}{n}
\]

Initially, it was hypothesized that the total sum is fixed at \( \sum\_{(u,v) \in E} w(u,v) \) for valid partitions, as each edge must be within exactly one clique due to disjointness, making the objective constant. This led to the conclusion that the task is to find any valid partition, not optimize a variable.

## Detailed Reasoning

Consider the condition "every input edge must be part of at least one clique." For disjoint cliques, an edge \( (u, v) \) is part of a clique \( C_i \) only if both \( u, v \in C_i \). Since cliques are disjoint, \( u \) and \( v \) must be in the same clique for \( (u, v) \) to be covered. This implies that the partition must be such that no edges exist between different cliques; otherwise, those edges would not be covered, violating the condition. Thus, the graph must be partitionable into disjoint cliques with no inter-cluster edges, effectively requiring \( G \) to be a cluster graph (disjoint union of cliques) for certain \( k \).

However, analysis of the sample input (e.g., \( n=5 \), edges like \( (0,1)=3 \), \( (0,4)=1 \), \( (3,4)=7 \)) shows connectivity (e.g., node 3 connected to 4, 4 connected to 0,1,2), suggesting it's not a cluster graph. For instance, trying to partition into \(\{0,1,2,4\}\) and \(\{3\}\) fails, as edge \( (3,4) \) is not covered (3 in \(\{3\}\), 4 in \(\{0,1,2,4\}\)). This raises questions about the feasibility for general graphs, especially with \( k \) constraints.

## Objective Value Analysis

Given the formula, if all edges are covered, the numerator

\[
\sum*{i=1}^M \sum*{(u,v) \in C_i} w(u,v)
\]

should equal

\[
\sum\_{(u,v) \in E} w(u,v)
\]

as each edge is in exactly one clique. The denominator is \( n \), so the objective is constant for valid partitions. This contradicts the optimization nature, suggesting either a misinterpretation or that the problem assumes graphs where such partitions exist, and the task is to find any valid one.

## Literature Review

Research on related problems, such as the **maximum edge clique partitioning problem**, shows it's NP-hard for graphs with clique number \( \geq 3 \), focusing on maximizing edges within cliques without size constraints. Our problem adds a size constraint \( k \) and requires all edges to be covered, aligning more with partitioning into cliques ensuring connectivity, but standard results suggest complexity for general graphs.

## Algorithmic Solution

Given the analysis, we propose a greedy approach, assuming the graph allows a valid partition:

### Graph Parsing

Use the weight matrix, where \( \text{weights}[i][j] = -9999 \) indicates no edge.

### Greedy Clique Construction

Iteratively:

1. Select an unassigned node \( u \) with the highest degree among unassigned nodes.
2. Start a clique with \( u \), then add up to \( k-1 \) unassigned neighbors that form a clique (connected to all current clique nodes).
3. Use intersection of neighbor lists to ensure completeness.
4. Add the clique to the partition, mark nodes as assigned.
5. Repeat until all nodes are assigned.

This ensures each clique is of size \( \leq k \) and a complete subgraph, but coverage of all edges depends on the graph's structure. For connected components not being cliques, partitioning may fail if \( k \) is too small (e.g., path of 3 nodes, \( k=2 \)), suggesting the problem assumes feasible inputs.

## Implementation Details

The C implementation involves dynamic memory allocation for cliques, using arrays to track assigned nodes and build candidates. Efficiency is managed by selecting high-degree nodes, with complexity \( O(n^2) \) per clique build for small \( n \), suitable for the problem's likely constraints.

## Example and Validation

For the sample input (\( n=5 \), edges like \( (0,1)=3 \), output "1 2 4"), analysis shows challenges in covering all edges (e.g., \( (3,4) \)), suggesting the sample may imply \( k \geq 4 \) and specific partitions. The greedy approach may yield \(\{0,1,2,4\}\) (if a clique, \( k \geq 4 \)) and \(\{3\}\), but edge coverage needs verification, highlighting potential input assumptions.

# Conclusion

The problem's objective appears constant for valid partitions, reducing to finding any partition into disjoint cliques of size \( \leq k \) covering all edges, requiring the graph to be partitionable without inter-cluster edges. The proposed greedy algorithm is practical for small \( n \), with feasibility depending on graph structure and \( k \). Further research into exact conditions for solvability is recommended, given potential NP-hardness for general cases.

# Key Citations

- A note on the complexity of the maximum edge clique partitioning problem with respect to the clique number
- Clique cover - Wikipedia
- Partition into cliques for cubic graphs: Planar case, complexity and approximation
