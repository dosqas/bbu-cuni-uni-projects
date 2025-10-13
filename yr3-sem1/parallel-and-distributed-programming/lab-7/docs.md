# Polynomial Multiplication Algorithms: Documentation (MPI C++ Version)

### Hint: run using the command: ```mpiexec -n 4 lab-7.exe```

## Implementation Note
For this lab, the algorithms were implemented in **C++ instead of C#**.  

- The reason for this change is that the **MPI library in C#** is not actively maintained, is third-party, and does not fully support **.NET 8.0**.  
- Specifically, the library internally uses **BinaryFormatter serialization**, which caused issues and could not be made to work with modern .NET versions.  
- Using C++ allows direct, well-supported access to **MPI (OpenMPI / MPICH)**, ensuring reliable parallel execution across multiple processes.

---

## 1. Algorithms

### Naive (O(n²)) Algorithm

The naive polynomial multiplication algorithm computes each coefficient of the result as the sum of products of pairs of coefficients whose indices add up to the same total. It performs **n × m** multiplications for input polynomials of degrees `n` and `m`.

#### **Sequential Version**
Implements the standard double-loop method:

```cpp
for (int i = 0; i < degree1; i++)
    for (int j = 0; j < degree2; j++)
        result[i + j] += poly1[i] * poly2[j];
````

This version runs entirely on a single process.

#### **MPI (Parallel) Version**

The input polynomial `poly1` is **partitioned among all available (in our case, we used 4) MPI processes**, with each process responsible for computing a subset of the resulting coefficients.

* Each process receives its **chunk** of indices based on:

  ```cpp
  int chunkSize = degree1 / size;
  int start = rank * chunkSize;
  int end = (rank == size - 1) ? degree1 : start + chunkSize;
  ```
* Each process independently multiplies its assigned portion of `poly1` with the entire `poly2`.
* Local results are accumulated in `localResult`.
* Finally, all processes combine their partial results using:

  ```cpp
  MPI_Reduce(localResult.data(), result, resultLength, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  ```

  The **root process (rank 0)** aggregates all partial sums into the final result.

---

### Karatsuba Algorithm (O(n^1.585))

Karatsuba’s algorithm reduces the number of recursive multiplications from 4 to 3 by splitting the polynomials into halves and combining results smartly.

If we split:

```
A(x) = A₀(x) + xᵏ A₁(x)
B(x) = B₀(x) + xᵏ B₁(x)
```

Then:

```
A(x)B(x) = z₀ + (z₁ - z₀ - z₂)xᵏ + z₂x²ᵏ
```

where:

* z₀ = A₀ × B₀
* z₂ = A₁ × B₁
* z₁ = (A₀ + A₁) × (B₀ + B₁)

#### **Sequential Version**

Uses recursion with the base case falling back to the naive algorithm when `n <= 32`:

```cpp
if (n <= 32) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i + j] += poly1[i] * poly2[j];
    return;
}
```

For larger sizes, it recursively computes `z₀`, `z₁`, and `z₂`, then combines them.

#### **MPI (Parallel) Version**

The Karatsuba algorithm is parallelized using **four MPI processes**:

* **Rank 0** acts as the **master** process, splitting data and aggregating results.
* **Ranks 1–3** compute the subproducts `z₀`, `z₂`, and `z₁`, respectively.

##### **Communication Flow:**

1. **Rank 0** splits polynomials into halves and computes intermediate sums:

   ```cpp
   sumA[i] = aLow[i] + aHigh[i];
   sumB[i] = bLow[i] + bHigh[i];
   ```
2. It sends these halves to worker processes:

   ```cpp
   MPI_Send(aLow.data(), k, MPI_INT, 1, 0, MPI_COMM_WORLD);
   MPI_Send(bLow.data(), k, MPI_INT, 1, 1, MPI_COMM_WORLD);
   ```

   (similar for ranks 2 and 3)
3. **Worker processes** compute their respective subproducts locally using the sequential Karatsuba recursion.
4. Each worker returns its result to **rank 0** using:

   ```cpp
   MPI_Send(localResult.data(), 2 * k, MPI_INT, 0, 100, MPI_COMM_WORLD);
   ```
5. **Rank 0** receives all results with:

   ```cpp
   MPI_Recv(z0.data(), 2 * k, MPI_INT, 1, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   ```

   and combines them into the final result.

Because each subproblem is independent, no additional synchronization beyond message passing is required.

---

## 2. Distribution and Communication

| Algorithm | Parallelization Strategy                 | Process Count               | Communication Method                    |
| --------- | ---------------------------------------- | --------------------------- | --------------------------------------- |
| Naive     | Data partitioning on `poly1` indices     | 4 processes | `MPI_Reduce` to sum partial results     |
| Karatsuba | Task-level decomposition into z0, z2, z1 | 4 processes                 | `MPI_Send` / `MPI_Recv` for subproblems |

**Notes:**

* **MPI_STATUS_IGNORE** is used in `MPI_Recv` when the program does not require information about the message source, tag, or error.
* The last process in the naive algorithm may handle a slightly larger chunk to include the remainder.

---

## 3. Performance Measurements

Execution times were measured using `std::chrono::high_resolution_clock` in C++.

### Small Polynomials (degree 8192)

**[NAIVE]**

```
Running naive approach (sequential)...
Time taken (naive sequential): 87 ms

Running naive approach (MPI)...
Time taken (naive MPI): 44 ms
```

**[KARATSUBA]**

```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 30 ms

Running Karatsuba algorithm (MPI)...
Time taken (Karatsuba MPI): 11 ms
```

### Large Polynomials (degree 65536)

**[NAIVE]**

```
Running naive approach (sequential)...
Time taken (naive sequential): 5576 ms

Running naive approach (MPI)...
Time taken (naive MPI): 2874 ms
```

**[KARATSUBA]**

```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 803 ms

Running Karatsuba algorithm (MPI)...
Time taken (Karatsuba MPI): 269 ms
```

---

## 4. Observations

* **Naive Sequential** is the slowest for both small and large polynomials.
* **Naive MPI** provides a speedup roughly proportional to the number of processes.
* **Karatsuba Sequential** is faster than naive approaches due to its reduced number of multiplications (O(n^1.585)).
* **Karatsuba MPI** is the fastest, benefiting from both algorithmic efficiency and parallel execution.
* MPI-based C++ implementation avoids C# limitations with third-party MPI libraries and serialization issues, enabling reliable and efficient parallel computation.

---

## Summary Table

| Algorithm | Parallelization        | Time Complexity | Notes                                              |
| --------- | ---------------------- | --------------- | -------------------------------------------------- |
| Naive     | MPI, partition `poly1` | O(n²)           | Synchronization via MPI_Reduce                     |
| Karatsuba | MPI, subproblem split  | O(n^1.585)      | Independent subproblems, combined via MPI messages |
