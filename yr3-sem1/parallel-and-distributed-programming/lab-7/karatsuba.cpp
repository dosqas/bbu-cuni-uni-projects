#include "Karatsuba.h"
#include <mpi.h>
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
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<int> result(2 * n, 0);

    if (isSequential) {
        if (rank == 0) {
            cout << "\nRunning Karatsuba algorithm (sequential)..." << endl;
            auto start = high_resolution_clock::now();
            KaratsubaRecursiveNonParallel(poly1, poly2, result.data(), n);
            auto stop = high_resolution_clock::now();
            cout << "Time taken (Karatsuba sequential): "
                << duration_cast<milliseconds>(stop - start).count() << " ms\n";
        }
    }
    else {
        if (rank == 0)
            cout << "\nRunning Karatsuba algorithm (MPI)..." << endl;
        KaratsubaMPI(poly1, poly2, n);
    }
}

// MPI parallel Karatsuba
void Karatsuba::KaratsubaMPI(const int* poly1, const int* poly2, int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int k = n / 2;
    vector<int> result(2 * n, 0);

    if (rank == 0) {
        auto start = high_resolution_clock::now();

        // Split into halves
        vector<int> aLow(poly1, poly1 + k);
        vector<int> aHigh(poly1 + k, poly1 + n);
        vector<int> bLow(poly2, poly2 + k);
        vector<int> bHigh(poly2 + k, poly2 + n);

        // Compute sums
        vector<int> sumA(k), sumB(k);
        for (int i = 0; i < k; i++) {
            sumA[i] = aLow[i] + aHigh[i];
            sumB[i] = bLow[i] + bHigh[i];
        }

        // Send subproblems to ranks 1, 2, 3
        MPI_Send(aLow.data(), k, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(bLow.data(), k, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Send(aHigh.data(), k, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(bHigh.data(), k, MPI_INT, 2, 1, MPI_COMM_WORLD);
        MPI_Send(sumA.data(), k, MPI_INT, 3, 0, MPI_COMM_WORLD);
        MPI_Send(sumB.data(), k, MPI_INT, 3, 1, MPI_COMM_WORLD);

        vector<int> z0(2 * k), z2(2 * k), z1(2 * k);

		// MPI_STATUS_IGNORE can be used when we don't care about the status
		// The status object can provide information about the received message
		// e.g. it's source, tag, and error code
		// In our case, we don't need that information
        MPI_Recv(z0.data(), 2 * k, MPI_INT, 1, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(z2.data(), 2 * k, MPI_INT, 2, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(z1.data(), 2 * k, MPI_INT, 3, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < 2 * k; i++)
            z1[i] -= z0[i] + z2[i];

        for (int i = 0; i < 2 * k; i++) result[i] += z0[i];
        for (int i = 0; i < 2 * k; i++) result[i + k] += z1[i];
        for (int i = 0; i < 2 * k; i++) result[i + 2 * k] += z2[i];

        auto stop = high_resolution_clock::now();
        cout << "Time taken (Karatsuba MPI): "
            << duration_cast<milliseconds>(stop - start).count() << " ms\n";
    }
    else if (rank <= 3) {
        vector<int> aSub(k), bSub(k);
        MPI_Recv(aSub.data(), k, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(bSub.data(), k, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> localResult(2 * k, 0);
        KaratsubaRecursiveNonParallel(aSub.data(), bSub.data(), localResult.data(), k);

        MPI_Send(localResult.data(), 2 * k, MPI_INT, 0, 100, MPI_COMM_WORLD);
    }
}
