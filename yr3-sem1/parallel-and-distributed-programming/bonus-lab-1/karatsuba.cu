#include "karatsuba.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

void Karatsuba::KaratsubaRecursiveNonParallel(const int* poly1, const int* poly2, int* result, int n) {
    if (n <= 32) {  
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                result[i + j] += poly1[i] * poly2[j];
        return;
    }

    int k = n / 2;

    vector<int> z0(2 * k, 0), z1(2 * k, 0), z2(2 * k, 0);
    vector<int> sumA(k, 0), sumB(k, 0);

    // z0 = poly1Low * poly2Low
    KaratsubaRecursiveNonParallel(poly1, poly2, z0.data(), k);

    // z2 = poly1High * poly2High
    KaratsubaRecursiveNonParallel(poly1 + k, poly2 + k, z2.data(), k);

    // sumA = poly1Low + poly1High
    // sumB = poly2Low + poly2High
    for (int i = 0; i < k; i++) {
        sumA[i] = poly1[i] + poly1[i + k];
        sumB[i] = poly2[i] + poly2[i + k];
    }

    // z1 = (sumA * sumB)
    KaratsubaRecursiveNonParallel(sumA.data(), sumB.data(), z1.data(), k);

    // z1 = z1 - z0 - z2
    for (int i = 0; i < 2 * k; i++)
        z1[i] -= z0[i] + z2[i];

    // Combine results
    for (int i = 0; i < 2 * k; i++) result[i] += z0[i];
    for (int i = 0; i < 2 * k; i++) result[i + k] += z1[i];
    for (int i = 0; i < 2 * k; i++) result[i + 2 * k] += z2[i];
}

void Karatsuba::KaratsubaAlgorithm(const int* poly1, const int* poly2, int n, bool isSequential) {
    vector<int> result(2 * n, 0);

    if (isSequential) {
        std::cout << "\nRunning Karatsuba algorithm (sequential)..." << endl;
        auto start = high_resolution_clock::now();
        KaratsubaRecursiveNonParallel(poly1, poly2, result.data(), n);
        auto stop = high_resolution_clock::now();
        std::cout << "Time taken (Karatsuba sequential): "
            << duration_cast<milliseconds>(stop - start).count() << " ms\n";
    }
    else {
        std::cout << "\nRunning Karatsuba algorithm (CUDA)..." << endl;
		auto start = high_resolution_clock::now();
        KaratsubaCUDA(poly1, poly2, result.data(), n);
		auto stop = high_resolution_clock::now();
        std::cout << "Time taken (Karatsuba CUDA): "
			<< duration_cast<milliseconds>(stop - start).count() << " ms\n";
    }
}

__global__ void karatsubaSingleSplitKernel(
    const int* poly1, const int* poly2, int* result,
    int n, int k)
{
    // Access the thread's unique index within the grid
    int i = blockIdx.x * blockDim.x + threadIdx.x;

	// Ensure we don't go out of bounds
	// (bounds = half the size of the polynomials)
    if (i < k) {
        // low/high halves
        int low1 = poly1[i];
        int high1 = poly1[i + k];
        int low2 = poly2[i];
        int high2 = poly2[i + k];

        atomicAdd(&result[i + i], low1 * low2); // z0
        atomicAdd(&result[i + i + k], low1 * high2 + high1 * low2); // z1
        atomicAdd(&result[i + i + 2 * k], high1 * high2); // z2
    }
}

void Karatsuba::KaratsubaCUDA(const int* h_poly1, const int* h_poly2, int* h_result, int n) {
    int* d_poly1, * d_poly2, * d_result;
    int resultSize = 2 * n - 1;

	// Allocate device memory
    cudaMalloc(&d_poly1, n * sizeof(int));
    cudaMalloc(&d_poly2, n * sizeof(int));
    cudaMalloc(&d_result, resultSize * sizeof(int));

	// Initialize result array on device to zero
    cudaMemset(d_result, 0, resultSize * sizeof(int));

    cudaMemcpy(d_poly1, h_poly1, n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_poly2, h_poly2, n * sizeof(int), cudaMemcpyHostToDevice);

    int k = n / 2;
    int threads = 256;
    int blocks = (k + threads - 1) / threads;

    karatsubaSingleSplitKernel<<<blocks, threads>>>(d_poly1, d_poly2, d_result, n, k);
    cudaDeviceSynchronize();

    cudaMemcpy(h_result, d_result, resultSize * sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_poly1);
    cudaFree(d_poly2);
    cudaFree(d_result);
}