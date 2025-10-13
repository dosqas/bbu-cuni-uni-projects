#include <mpi.h>
#include <iostream>
#include <random>
#include "Naive.h"
#include "Karatsuba.h"

using namespace std;

static int* GeneratePolynomial(int degree, bool largeNumbers) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(largeNumbers ? -1000 : -10, largeNumbers ? 1000 : 10);

    int* polynomial = new int[degree];
    for (int i = 0; i < degree; i++)
        polynomial[i] = dis(gen);

    return polynomial;
}

static void SmallPolynomials() {
    int degree = 8192;
    int* poly1 = GeneratePolynomial(degree, false);
    int* poly2 = GeneratePolynomial(degree, false);
    int* result = new int[degree * 2];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        cout << "[NAIVE]" << endl;
        Naive::Sequential(poly1, poly2, degree, degree, result);
    }

    Naive::MPI(poly1, poly2, degree, degree, result);

    if (rank == 0)
        cout << "\n\n[KARATSUBA]" << endl;

    if (rank == 0)
        Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, true);

    Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, false);

    delete[] poly1;
    delete[] poly2;
}

static void LargePolynomials() {
    int degree = 65536;
    int* poly1 = GeneratePolynomial(degree, true);
    int* poly2 = GeneratePolynomial(degree, true);
    int* result = new int[degree * 2];

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        cout << "[NAIVE]" << endl;
        Naive::Sequential(poly1, poly2, degree, degree, result);
    }

    Naive::MPI(poly1, poly2, degree, degree, result);

    if (rank == 0)
        cout << "\n\n[KARATSUBA]" << endl;

    if (rank == 0)
        Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, true);

    Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, false);

    delete[] poly1;
    delete[] poly2;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        cout << "=== SMALL POLYNOMIALS ===" << endl;

    SmallPolynomials();

    if (rank == 0)
        cout << "\n\n\n\n=== LARGE POLYNOMIALS ===" << endl;

    LargePolynomials();

    MPI_Finalize();
    return 0;
}
