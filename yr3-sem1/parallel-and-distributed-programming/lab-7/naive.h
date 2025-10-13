#pragma once
#include <vector>

class Naive {
public:
    static void Sequential(const int* poly1, const int* poly2, int degree1, int degree2, int* result);

    static void MPI(const int* poly1, const int* poly2, int degree1, int degree2, int* result);
};
