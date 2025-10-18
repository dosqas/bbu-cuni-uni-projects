#include <iostream>
#include <random>
#include "naive.cuh"
#include "karatsuba.cuh"

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

    cout << "[NAIVE]" << endl;
    Naive::Sequential(poly1, poly2, degree, degree, result);
    Naive::CUDA(poly1, poly2, degree, degree, result);

    cout << "\n\n[KARATSUBA]" << endl;
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

    cout << "[NAIVE]" << endl;
    Naive::Sequential(poly1, poly2, degree, degree, result);
    Naive::CUDA(poly1, poly2, degree, degree, result);

    cout << "\n\n[KARATSUBA]" << endl;
    Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, true);
    Karatsuba::KaratsubaAlgorithm(poly1, poly2, degree, false);

    delete[] poly1;
    delete[] poly2;
}

int main(int argc, char** argv) {
    cout << "=== SMALL POLYNOMIALS ===" << endl;
    SmallPolynomials();

    cout << "\n\n\n\n=== LARGE POLYNOMIALS ===" << endl;
    LargePolynomials();

    return 0;
}
