#include "naive.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <chrono>

using namespace std::chrono;

void Naive::Sequential(const int* poly1, const int* poly2, int degree1, int degree2, int* result) {
    std::cout << "\nRunning naive approach (sequential)..." << std::endl;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < degree1; i++) {
        for (int j = 0; j < degree2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }

    auto stop = high_resolution_clock::now();
    std::cout << "Time taken (naive sequential): "
        << duration_cast<milliseconds>(stop - start).count() << " ms\n";
}

// Function runs on the GPU, launched by the host (CPU)
__global__ void polyMultiplyKernelNaive(const int* poly1, const int* poly2, int* result, int degree1, int degree2) {
	// Calculate global thread indices
	// Each thread has acces to its unique indices within its block (threadIdx) and the block's indices within the grid (blockIdx)
	// To access its block, we multiply blockIdx by blockDim (number of threads per block)
	// and add threadIdx to get the index of the thread within the entire grid
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

	// If indices are within bounds, perform multiplication and accumulate result
	// To synchronize access to shared memory, we use atomicAdd to prevent
	// race conditions
    if (i < degree1 && j < degree2) {
        atomicAdd(&result[i + j], poly1[i] * poly2[j]);
    }
}

void Naive::CUDA(const int* h_poly1, const int* h_poly2, int degree1, int degree2, int* h_result) {
    int resultLength = degree1 + degree2 - 1;

    // Allocate device memory
	// The GPU has its own memory space, so we need to allocate memory on it
    int* d_poly1, * d_poly2, * d_result;
    cudaMalloc(&d_poly1, degree1 * sizeof(int));
    cudaMalloc(&d_poly2, degree2 * sizeof(int));
    cudaMalloc(&d_result, resultLength * sizeof(int));

    // Copy data from host to device
    cudaMemcpy(d_poly1, h_poly1, degree1 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_poly2, h_poly2, degree2 * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemset(d_result, 0, resultLength * sizeof(int));

    std::cout << "\nRunning naive approach (CUDA)..." << std::endl;
    auto start = high_resolution_clock::now();

    // Define CUDA grid and block dimensions
	// Thread - smallest units of execution
	// Block - group of threads that can cooperate with each other
	// Grid - group of blocks
	// 16 x 16 = 256 threads per block
	// polynomial_degree + 15 / 16 to round down to nearest integer (ensure enough blocks to cover all coefficients)
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((degree1 + 15) / 16, (degree2 + 15) / 16);

    // Launch kernel
	// <<< ... >>> = execution configuration syntax
	// We launch it with the specified number of blocks and threads per block
    polyMultiplyKernelNaive<<<numBlocks, threadsPerBlock>>>(d_poly1, d_poly2, d_result, degree1, degree2);

	// Wait for GPU to finish before accessing on host
    cudaDeviceSynchronize();

    auto stop = high_resolution_clock::now();
    std::cout << "Time taken (naive CUDA): "
        << duration_cast<milliseconds>(stop - start).count() << " ms\n";

    // Copy result back to host
    cudaMemcpy(h_result, d_result, resultLength * sizeof(int), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_poly1);
    cudaFree(d_poly2);
    cudaFree(d_result);
}
