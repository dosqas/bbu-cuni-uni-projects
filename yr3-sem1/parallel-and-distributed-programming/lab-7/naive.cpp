#include "Naive.h"
#include <mpi.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void Naive::Sequential(const int* poly1, const int* poly2, int degree1, int degree2, int* result) {
    cout << "\nRunning naive approach (sequential)..." << endl;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < degree1; i++) {
        for (int j = 0; j < degree2; j++) {
            result[i + j] += poly1[i] * poly2[j];
        }
    }

    auto stop = high_resolution_clock::now();
    cout << "Time taken (naive sequential): "
        << duration_cast<milliseconds>(stop - start).count() << " ms\n";
}

void Naive::MPI(const int* poly1, const int* poly2, int degree1, int degree2, int* result) {
    int resultLength = degree1 + degree2 - 1;

    int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Get current process rank
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Get total number of processes

	int chunkSize = degree1 / size; // Size of chunk for each process
    int start = rank * chunkSize;
	int end = (rank == size - 1) ? degree1 : start + chunkSize; // Last process may take the remainder

    // Local buffer for each process
    vector<int> localResult(resultLength, 0);

    if (rank == 0)
        cout << "\nRunning naive approach (MPI)..." << endl;

    auto mpiStart = high_resolution_clock::now();

    for (int i = start; i < end; i++) {
        for (int j = 0; j < degree2; j++) {
            localResult[i + j] += poly1[i] * poly2[j];
        }
    }

    // Reduce all local results into the global result
    MPI_Reduce(localResult.data(), result, resultLength, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        auto mpiStop = high_resolution_clock::now();
        cout << "Time taken (naive MPI): "
            << duration_cast<milliseconds>(mpiStop - mpiStart).count() << " ms\n";
    }
}
