# Hamiltonian Cycle Algorithms: Documentation

## 1. Algorithms

### Sequential Hamiltonian Cycle Search

Both C# and Java implementations use a depth-first search (DFS) to find a Hamiltonian cycle in a directed graph. The algorithm recursively explores all possible paths, marking nodes as visited and backtracking when necessary. If a path visits every node exactly once and returns to the starting node, a Hamiltonian cycle is found.

- **C#:**  
  The sequential logic is embedded in the recursive `ParallelHamiltonianDFS` method when only one thread is available.
- **Java:**  
  The sequential logic is in the `HamiltonianCycleTask.compute()` method when only one thread is left for allocation.

### Parallel Hamiltonian Cycle Search

Both implementations parallelize the DFS by distributing available threads among branches at each recursion level.

- **C#:**  
  Uses `Task.Run` to spawn parallel tasks for each branch when multiple threads are available. The `DistributeThreads` helper evenly allocates threads to branches.
- **Java:**  
  Uses the Fork/Join framework (`RecursiveTask`) to spawn parallel tasks for each branch. The `allocateThreads` method evenly distributes threads among branches.

---

## 2. Synchronization in Parallelized Variants

- **C#:**  
  - Uses `Volatile.Read` to check if a cycle has already been found, ensuring visibility across threads.
  - Uses `lock` on the shared `Result` object to safely update the result when a cycle is found.
  - Each recursive branch gets its own copy of the visited array and path, avoiding shared state issues.

- **Java:**  
  - Fork/Join tasks operate on copies of visited sets and paths, so no explicit locking is needed for those.
  - The result is returned up the call stack; the first non-null result is accepted.
  - The ForkJoinPool manages task execution and synchronization.

---

## 3. Performance Measurements

Both implementations can be instrumented to measure execution time for sequential and parallel variants.

- **C#:**  
  Use `Stopwatch` to measure the time taken for the search.
- **Java:**  
  Use `System.nanoTime()` to measure execution time.

**Comparison:**
- Sequential DFS is slow for large graphs due to exponential complexity.
- Parallel DFS improves performance by exploring branches concurrently, especially on multi-core CPUs.
- Synchronization overhead is minimal since most state is thread-local, but thread management and task creation add some overhead.
- For small graphs, parallel overhead may outweigh benefits; for larger graphs, parallelization can significantly reduce runtime.

---

### Example Performance Results

**[C#]**
```
Starting parallel Hamiltonian cycle search with 8 threads...

Hamiltonian cycle found:
0 -> 5 -> 4 -> 8 -> 9 -> 2 -> 1 -> 7 -> 3 -> 6 -> 0
Time taken: 2 ms
```

**[Java]**
```
Starting parallel Hamiltonian cycle search with 8 threads...
Cycle found: [0, 2, 3, 6, 7, 8, 5, 1, 9, 4, 0]
Time taken: 3 ms
```

---

## Summary Table

| Approach             | Synchronization        | Parallelism Mechanism  |
|----------------------|------------------------|------------------------| 
| Sequential DFS       | N/A                    | N/A                    | 
| Parallel DFS (C#)    | lock, Volatile.Read    | Task.Run, WaitAll      | 
| Parallel DFS (Java)  | ForkJoinPool           | invokeAll, join        | 

---

**Note:**  
Actual performance depends on graph size, density, hardware, and runtime optimizations. For small graphs, parallel overhead may outweigh benefits. For large graphs, parallel DFS is typically much faster than sequential DFS, but still exponential in worst case.