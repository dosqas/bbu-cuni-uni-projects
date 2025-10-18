# Polynomial Multiplication Algorithms: Documentation (CUDA C++ Version)

## Implementation Note

For this lab, the algorithms were implemented in **C++ with CUDA**.

* Using CUDA allows efficient **parallel execution on GPUs**, leveraging thousands of threads to accelerate polynomial multiplication.
* Both the **naive O(n²) algorithm** and a **single-split Karatsuba variant** were implemented on the GPU.
* CUDA kernels use `atomicAdd` to safely accumulate partial results when multiple threads write to the same output coefficient.

---

## 1. Algorithms

### Naive (O(n²)) Algorithm

The naive polynomial multiplication algorithm computes each coefficient of the result as the sum of products of pairs of coefficients whose indices add up to the same total. It performs **n × m** multiplications for input polynomials of degrees `n` and `m`.

#### **Sequential Version**

```cpp
for (int i = 0; i < degree1; i++)
    for (int j = 0; j < degree2; j++)
        result[i + j] += poly1[i] * poly2[j];
```

#### **CUDA (Parallel) Version**

* Each thread is responsible for computing one combination of indices `(i,j)`:

```cpp
__global__ void polyMultiplyKernelNaive(const int* poly1, const int* poly2, int* result, int degree1, int degree2) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < degree1 && j < degree2) {
        atomicAdd(&result[i + j], poly1[i] * poly2[j]);
    }
}
```

* Threads are organized in a 2D grid. `atomicAdd` ensures correct accumulation of overlapping results.

---

### Karatsuba Algorithm (Single-Split, O(n^1.585))

The single-split Karatsuba algorithm reduces the number of multiplications from 4 to 3 by splitting the polynomial into two halves and combining results smartly:

```
A(x) = A_low + x^k A_high
B(x) = B_low + x^k B_high
```

Then:

```
A(x)B(x) = z0 + (z1 - z0 - z2)x^k + z2 x^(2k)
```

where:

* `z0 = A_low * B_low`
* `z2 = A_high * B_high`
* `z1 = (A_low + A_high) * (B_low + B_high)`

#### **CUDA (Parallel) Version**

* In a single GPU kernel, we compute `z0`, `z1`, and `z2` contributions **simultaneously** for all indices in the first half:

```cpp
__global__ void karatsubaSingleSplitKernel(
    const int* poly1, const int* poly2, int* result,
    int n, int k)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < k) {
        int low1 = poly1[i];
        int high1 = poly1[i + k];
        int low2 = poly2[i];
        int high2 = poly2[i + k];

        atomicAdd(&result[i + i], low1 * low2);           // z0
        atomicAdd(&result[i + i + k], low1 * high2 + high1 * low2); // z1
        atomicAdd(&result[i + i + 2*k], high1 * high2);  // z2
    }
}
```

* The host code allocates GPU memory, copies data, launches the kernel, and retrieves results.

```cpp
int k = n / 2;
int threads = 256;
int blocks = (k + threads - 1) / threads;

karatsubaSingleSplitKernel<<<blocks, threads>>>(d_A, d_B, d_C, n, k);
cudaDeviceSynchronize();
```

* This method avoids recursion and performs the first-level Karatsuba split entirely on the GPU.

---

## 2. Performance Measurements

Execution times were measured using `std::chrono::high_resolution_clock`.

### Small Polynomials (n = 8192)

**[NAIVE]**

```
Running naive approach (sequential)...
Time taken (naive sequential): 92 ms

Running naive approach (CUDA)...
Time taken (naive CUDA): 11 ms
```

**[KARATSUBA]**

```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 23 ms

Running Karatsuba algorithm (CUDA)...
Time taken (Karatsuba CUDA): 1 ms
```

### Large Polynomials (n = 65536)

**[NAIVE]**

```
Running naive approach (sequential)...
Time taken (naive sequential): 5513 ms

Running naive approach (CUDA)...
Time taken (naive CUDA): 604 ms
```

**[KARATSUBA]**

```
Running Karatsuba algorithm (sequential)...
Time taken (Karatsuba sequential): 626 ms

Running Karatsuba algorithm (CUDA)...
Time taken (Karatsuba CUDA): 1 ms
```

---

## 3. Observations

* **Naive Sequential** is the slowest for all sizes.
* **Naive CUDA** provides a significant speedup due to massive thread-level parallelism.
* **Karatsuba Sequential** reduces computation with fewer multiplications.
* **Single-Split Karatsuba CUDA** is the fastest, leveraging both algorithmic efficiency and GPU parallelism.
* `atomicAdd` ensures correctness when multiple threads update overlapping coefficients.

---

## 4. Summary Table

| Algorithm | Parallelization         | Time Complexity | Notes                                  |
| --------- | ----------------------- | --------------- | -------------------------------------- |
| Naive     | CUDA threads on (i,j)   | O(n²)           | `atomicAdd` for accumulation           |
| Karatsuba | Single-split GPU kernel | O(n^1.585)      | z0, z1, z2 computed in one step on GPU |
