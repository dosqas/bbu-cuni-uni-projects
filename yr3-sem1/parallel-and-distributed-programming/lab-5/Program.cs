namespace lab_5;

class Program 
{
    static void Main() 
    { 
        Console.WriteLine("=== SMALL POLYNOMIALS ===");
        SmallPolynomials();

        // For the bonus, we will run the same algorithms on larger polynomials
        // (both in terms of degree and coefficient size)
        Console.WriteLine("\n\n\n\n=== LARGE POLYNOMIALS ===");
        LargePolynomials();
    }

    static int[] GeneratePolynomial(int degree, string polyName, bool largeNumbers)
    {
        Random rand = new();
        int[] polynomial = new int[degree];

        // Console.Write(polyName + " = ");
        for (int i = 0; i < degree; i++)
        {
            polynomial[i] = largeNumbers ? rand.Next(-1000, 1001) : rand.Next(-10, 11);
            // Console.Write(polynomial[i] + (i < degree - 1 ? "x^" + i + " + " : ""));
        }
        return polynomial;
    }

    static void SmallPolynomials()
    {
        // 2^13 = 8192; for simplicity's sake
        int[] poly1 = GeneratePolynomial(8192, "poly1", largeNumbers: false);
        int[] poly2 = GeneratePolynomial(8192, "poly2", largeNumbers: false);

        Console.WriteLine("[NAIVE]");
        Naive.Sequential(poly1, poly2);
        Naive.Parallelized(poly1, poly2);

        Console.WriteLine("\n\n[KARATSUBA]");
        Karatsuba.KaratsubaAlgorithm(poly1, poly2, isSequential: true);
        Karatsuba.KaratsubaAlgorithm(poly1, poly2, isSequential: false);
    }

    static void LargePolynomials()
    {
        // 2^16 = 65536; for simplicity's sake
        int[] poly1 = GeneratePolynomial(65536, "poly1", largeNumbers: true);
        int[] poly2 = GeneratePolynomial(65536, "poly2", largeNumbers: true);

        Console.WriteLine("[NAIVE]");
        Naive.Sequential(poly1, poly2);
        Naive.Parallelized(poly1, poly2);

        Console.WriteLine("\n\n[KARATSUBA]");
        Karatsuba.KaratsubaAlgorithm(poly1, poly2, isSequential: true);
        Karatsuba.KaratsubaAlgorithm(poly1, poly2, isSequential: false);
    }
}