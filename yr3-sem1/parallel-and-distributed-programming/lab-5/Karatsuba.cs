using System.Diagnostics;
using System.Drawing;

namespace lab_5;

class Karatsuba
{
    // If the polynomial size is above this threshold, we will parallelize the recursive calls
    // Otherwise, we will do them sequentially to avoid excessive overhead from task management
    const int PARALLEL_THRESHOLD = 32;

    public static void KaratsubaAlgorithm(int[] poly1, int[] poly2, bool isSequential)
    {
        int[] result = new int[2 * poly1.Length];
        Stopwatch sw = new();

        if (isSequential)
        {
            Console.WriteLine("\nRunning Karatsuba algorithm (sequential)...");
            sw.Start();
            KaratsubaRecursiveNonParallel(poly1, poly2, result, poly1.Length);
        }
        else
        {
            Console.WriteLine("\nRunning Karatsuba algorithm (parallel)...");
            sw.Start();
            KaratsubaRecursiveParallel(poly1, poly2, result, poly1.Length);
        }

        sw.Stop();
        if (isSequential) {
            Console.WriteLine($"Time taken (Karatsuba sequential): {sw.ElapsedMilliseconds} ms");
        }
        else
            Console.WriteLine($"Time taken (Karatsuba parallel): {sw.ElapsedMilliseconds} ms");
    }

    private static void KaratsubaRecursiveNonParallel(int[] poly1, int[] poly2, int[] result, int n)
    {
        if (n <= 2)
        {
            // Base case: naive multiplication
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    result[i + j] += poly1[i] * poly2[j];

            return;
        }

        int k = n / 2; // We split the polynomials in half

        int[] poly1Low = new int[k];
        int[] poly1High = new int[k];
        int[] poly2Low = new int[k];
        int[] poly2High = new int[k];

        Array.Copy(poly1, 0, poly1Low, 0, k); // Copy first half of poly1 to poly1Low
        Array.Copy(poly1, k, poly1High, 0, k);
        Array.Copy(poly2, 0, poly2Low, 0, k);
        Array.Copy(poly2, k, poly2High, 0, k);

        int[] z0 = new int[2 * k];
        int[] z1 = new int[2 * k];
        int[] z2 = new int[2 * k];

        // z0 = poly1Low * poly2Low
        KaratsubaRecursiveNonParallel(poly1Low, poly2Low, z0, k);

        // z2 = poly1High * poly2High
        KaratsubaRecursiveNonParallel(poly1High, poly2High, z2, k);

        // z1 = (poly1Low + poly1High) * (poly2Low + poly2High) - z0 - z2
        //    = poly1Low*poly2Low + poly1Low*poly2High + poly1High*poly2Low + poly1High*poly2High - z0 - z2
        //    = poly1Low*poly2Low + poly1Low*poly2High + poly1High*poly2Low + poly1High*poly2High - poly1Low*poly2Low - poly1High*poly2High
        //    = poly1Low*poly2High + poly1High*poly2Low
        // As such we only need to compute 3 half-size multiplications instead of 4
        // The time it takes to do the additions and subtractions is O(n)
        // As such we save a factor of n in the multiplication step
        // This is what gives Karatsuba its O(n^log2(3)) = O(n^1.585) time complexity
        // (as opposed to O(n^2) for naive multiplication)
        int[] sumPoly1 = new int[k];
        int[] sumPoly2 = new int[k];
        for (int i = 0; i < k; i++)
        {
            sumPoly1[i] = poly1Low[i] + poly1High[i];
            sumPoly2[i] = poly2Low[i] + poly2High[i];
        }

        KaratsubaRecursiveNonParallel(sumPoly1, sumPoly2, z1, k);

        // We calculated the (poly1Low + poly1High) * (poly2Low + poly2High) part, now we need to subtract z0 and z2
        for (int i = 0; i < z1.Length; i++)
            z1[i] = z1[i] - z0[i] - z2[i];

        // Combine: result = z0 + (z1 << k) + (z2 << (2*k))
        // (<< k means multiply by x^k, which is equivalent to shifting the coefficients)
        // In other words, result = z0 + z1*x^k + z2*x^(2*k)
        for (int i = 0; i < z0.Length; i++) result[i] += z0[i];
        for (int i = 0; i < z1.Length; i++) result[i + k] += z1[i];
        for (int i = 0; i < z2.Length; i++) result[i + 2 * k] += z2[i];
    }

    public static void KaratsubaRecursiveParallel(int[] poly1, int[] poly2, int[] result, int n)
    {
        // Base case: small polynomials, use naive multiplication
        if (n <= PARALLEL_THRESHOLD) // small enough for sequential
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    result[i + j] += poly1[i] * poly2[j];
                }
            }
            return;
        }

        int k = n / 2;

        // Split a and b into low and high parts
        int[] poly1Low = new int[k];
        int[] poly1High = new int[k];
        int[] poly2Low = new int[k];
        int[] poly2High = new int[k];

        Array.Copy(poly1, 0, poly1Low, 0, k);
        Array.Copy(poly1, k, poly1High, 0, k);
        Array.Copy(poly2, 0, poly2Low, 0, k);
        Array.Copy(poly2, k, poly2High, 0, k);

        int[] z0 = new int[2 * k];
        int[] z1 = new int[2 * k];
        int[] z2 = new int[2 * k];

        // Compute z0 and z2 in parallel if large enough
        Task? taskZ0 = null, taskZ2 = null;

        if (n >= PARALLEL_THRESHOLD)
        {
            taskZ0 = Task.Run(() => KaratsubaRecursiveParallel(poly1Low, poly2Low, z0, k));
            taskZ2 = Task.Run(() => KaratsubaRecursiveParallel(poly1High, poly2High, z2, k));
        }
        else
        {
            KaratsubaRecursiveParallel(poly1Low, poly2Low, z0, k);
            KaratsubaRecursiveParallel(poly1High, poly2High, z2, k);
        }

        // Compute (poly1Low+poly1High)*(poly2Low+poly2High) -> z1
        int[] sumA = new int[k];
        int[] sumB = new int[k];

        for (int i = 0; i < k; i++)
        {
            sumA[i] = poly1Low[i] + poly1High[i];
            sumB[i] = poly2Low[i] + poly2High[i];
        }

        KaratsubaRecursiveParallel(sumA, sumB, z1, k);

        if (taskZ0 != null && taskZ2 != null)
            Task.WaitAll(taskZ0, taskZ2);

        // z1 = z1 - z0 - z2
        for (int i = 0; i < z1.Length; i++)
            z1[i] = z1[i] - z0[i] - z2[i];

        // Combine results into the final result
        for (int i = 0; i < z0.Length; i++)
            result[i] += z0[i];
        for (int i = 0; i < z1.Length; i++)
            result[i + k] += z1[i];
        for (int i = 0; i < z2.Length; i++)
            result[i + 2 * k] += z2[i];
    }
}
