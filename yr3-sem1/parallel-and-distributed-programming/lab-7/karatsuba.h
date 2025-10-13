#pragma once
#include <mpi.h>

class Karatsuba {
public:
    static void KaratsubaAlgorithm(const int* poly1, const int* poly2, int n, bool isSequential);
    static void KaratsubaMPI(const int* poly1, const int* poly2, int n);

private:
    static void KaratsubaRecursiveNonParallel(const int* poly1, const int* poly2, int* result, int n);
};
