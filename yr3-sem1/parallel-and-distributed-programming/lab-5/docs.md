# Polynomial Multiplication Algorithms: Documentation

## 1. Algorithms

### Naive (Regular O(n²)) Algorithm
The naive approach multiplies two polynomials by iterating over all pairs of coefficients. For polynomials of degree n and m, it computes each coefficient of the result as the sum of products of coefficients whose indices add up to the target index.

**Sequential:**  
Loops through all pairs `(i, j)` and accumulates `poly1[i] * poly2[j]` into `result[i + j]`.

**Parallelized:**  
Uses `Parallel.For` to distribute the outer loop (`i`) across threads. Synchronization is achieved through the use of the `Interlocked` class: 

```Interlocked.Add(ref result[i + j], poly1[i] * poly2[j]);```

We make the add operation atomic (as such making it thread-safe), and add into `result[i+j]` the result of `poly1[i] * poly2[j]`

---

### Karatsuba Algorithm (O(n^~1.585))
Karatsuba is a divide-and-conquer algorithm that reduces the number of recursive multiplications from 4 to 3 by cleverly combining subproblems. It splits each polynomial into two halves and recursively computes three products, then combines them.

**Sequential:**  
Recursively splits polynomials and computes subproducts without parallelism.

**Parallelized:**  
Recursively splits polynomials, but for sufficiently large subproblems (controllable through the `PARALLEL_THRESHOLD` hyperparameter), launches recursive calls for subproducts (`z0`, `z2`) in parallel using `Task.Run`. Synchronization is handled by waiting for tasks to complete before combining results.

---

## 2. Synchronization in Parallelized Variants

- **Naive Parallelized:**  
  Uses `Interlocked.Add` to safely update shared elements of the result array, preventing race conditions when multiple threads write to the same index.

- **Karatsuba Parallelized:**  
  Uses `Task.Run` to execute recursive calls in parallel. Synchronization is achieved by `Task.WaitAll` to ensure all parallel tasks finish before combining results. Since each subproblem writes to its own result array, no explicit locking is needed for those arrays.

---

## 3. Performance Measurements

Each algorithm variant measures execution time using `Stopwatch`:

- **Naive Sequential:**  
  Measures time for the single-threaded nested loop.

- **Naive Parallelized:**  
  Measures time for the parallelized outer loop.

- **Karatsuba Sequential:**  
  Measures time for the recursive, single-threaded divide-and-conquer approach.

- **Karatsuba Parallelized:**  
  Measures time for the recursive approach with parallel subproblem execution.

**Comparison:**  
- Naive sequential is slowest for both small and large polynomials.
- Naive parallelized improves performance by utilizing multiple cores.
- Karatsuba sequential is faster than both naive approaches due to reduced multiplications.
- Karatsuba parallelized is the fastest approach for both polynomial sizes tested, benefiting from both algorithmic efficiency and parallel execution.

---

### Example Performance Results

#### === SMALL POLYNOMIALS ===

**[NAIVE]**
```
Running naive approach (sequential)...
Time taken (naive sequential): 109 ms

Running naive approach (parallelized)...
Time taken (naive parallelized): 67 ms
```

**[KARATSUBA]**
```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 42 ms

Running Karatsuba algorithm (parallel)...
Time taken (Karatsuba parallel): 5 ms
```

#### === LARGE POLYNOMIALS ===

**[NAIVE]**
```
Running naive approach (sequential)...
Time taken (naive sequential): 6912 ms

Running naive approach (parallelized)...
Time taken (naive parallelized): 5501 ms
```

**[KARATSUBA]**
```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 1691 ms

Running Karatsuba algorithm (parallel)...
Time taken (Karatsuba parallel): 181 ms
```

---

**Summary Table:**

| Algorithm         | Synchronization        | Time Complexity |
|-------------------|------------------------|-----------------|
| Naive             | Interlocked.Add        | O(n²)           |
| Karatsuba         | Task.WaitAll           | O(n^1.585)      |

---

**Note:**  
Actual performance depends on polynomial size, hardware, and .NET runtime optimizations. For small polynomials, parallel overhead may outweigh benefits. For large polynomials, parallel Karatsuba is typically fastest.