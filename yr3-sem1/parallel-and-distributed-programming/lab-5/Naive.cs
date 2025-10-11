using System.Diagnostics;

namespace lab_5;

class Naive
{
    public static void Sequential(int[] poly1, int[] poly2)
    {
        int degree1 = poly1.Length;
        int degree2 = poly2.Length;
        int[] result = new int[degree1 + degree2 - 1];

        Console.WriteLine("\nRunning naive approach (sequential)...");
        Stopwatch stopwatch = new();
        stopwatch.Start();
        for (int i = 0; i < degree1; i++)
        {
            for (int j = 0; j < degree2; j++)
            {
                result[i + j] += poly1[i] * poly2[j];
            }
        }
        stopwatch.Stop();
        Console.WriteLine($"Time taken (naive sequential): {stopwatch.ElapsedMilliseconds} ms");
    }

    public static void Parallelized(int[] poly1, int[] poly2)
    {
        int degree1 = poly1.Length;
        int degree2 = poly2.Length;
        int[] result = new int[degree1 + degree2 - 1];

        Console.WriteLine("\nRunning naive approach (parallelized)...");
        Stopwatch stopwatch = new();
        stopwatch.Start();
        Parallel.For(0, degree1, i =>
        {
            for (int j = 0; j < degree2; j++)
            {
                // Interlocked makes the addition be atomic, as such making it thread-safe
                // From the documentation: "Provides atomic operations for variables that are shared by multiple threads."
                // The .Add method "Adds two 32-bit integers and replaces the first integer with the sum, as an atomic operation."
                Interlocked.Add(ref result[i + j], poly1[i] * poly2[j]);
            }
        });
        stopwatch.Stop();
        Console.WriteLine($"Time taken (naive parallelized): {stopwatch.ElapsedMilliseconds} ms");
    }
}
