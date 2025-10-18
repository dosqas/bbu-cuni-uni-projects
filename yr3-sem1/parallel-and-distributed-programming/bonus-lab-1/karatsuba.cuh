#pragma once

class Karatsuba {
public:
    static void KaratsubaAlgorithm(const int* poly1, const int* poly2, int n, bool isSequential);
    static void KaratsubaCUDA(const int* poly1, const int* poly2, int* result, int n);

private:
    static void KaratsubaRecursiveNonParallel(const int* poly1, const int* poly2, int* result, int n);
};
