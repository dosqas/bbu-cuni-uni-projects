using System.Diagnostics;

namespace lab_3;

class Program
{
    static int matrixSize;
    static readonly int functionVersion = 3; 

    static int[,]? matrixA;
    static int[,]? matrixB;
    static int[,]? resultMatrix;

    static int Compute_element(int row, int col, int thread_number)
    {
        Console.WriteLine($"Thread {thread_number} computing element [{row}, {col}]");

        int sum = 0;
        for (int k = 0; k < matrixSize; k++)
        {
            sum += matrixA![row, k] * matrixB![k, col];
        }
        return sum;
    }

    static void Compute_product_row_by_row(int startIdx, int endIdx, int thread_number)
    {
        for (int k = startIdx; k < endIdx; k++)
        {
            resultMatrix![k / matrixSize, k % matrixSize] = Compute_element(k / matrixSize, k % matrixSize, thread_number);
        }
    }

    static void Compute_product_col_by_col(int startIdx, int endIdx, int thread_number)
    {
        for (int k = startIdx; k < endIdx; k++)
        {
            resultMatrix![k % matrixSize, k / matrixSize] = Compute_element(k % matrixSize, k / matrixSize, thread_number);
        }
    }

    static void Compute_product_every_k(int startIdx, int k, int amount, int thread_number)
    {
        for (int i = 0; i < amount; i++)
        {
            int index = startIdx + i * k;
            resultMatrix![index / matrixSize, index % matrixSize] = Compute_element(index / matrixSize, index % matrixSize, thread_number);
        }
    }

    static void Generate_matrices()
    {
        matrixA = new int[matrixSize, matrixSize];
        matrixB = new int[matrixSize, matrixSize];
        resultMatrix = new int[matrixSize, matrixSize];

        var rand = new Random();
        for (int i = 0; i < matrixSize; i++)
        {
            for (int j = 0; j < matrixSize; j++)
            {
                matrixA[i, j] = rand.Next(1, 4);
                matrixB[i, j] = rand.Next(1, 4);
            }
        }
    }

    static void Main()
    {
        for (int thread_count = 4; thread_count <= 10; thread_count+=2)
        {
            Console.WriteLine("===============================================");
            Console.WriteLine($"Starting experiment with {thread_count} threads");
            for (int matrix_size = 10; matrix_size <= 11; matrix_size++)
            {
                matrixSize = matrix_size;
                Console.WriteLine($"\n[{matrix_size - 9}] Matrix size: {matrixSize}x{matrixSize}");
                Generate_matrices();

                var tasks = new List<Task>();

                var stopwatch = Stopwatch.StartNew();

                for (int k = 0; k < thread_count; k++)
                {
                    int threadNum = k;
                    int startIdx = threadNum * (matrixSize * matrixSize) / thread_count;
                    int endIdx = (threadNum + 1) * (matrixSize * matrixSize) / thread_count;
                    var t = new Task(() =>
                    {
                        switch (functionVersion)
                        {
                            case 1:
                                Compute_product_row_by_row(startIdx, endIdx, threadNum);
                                break;
                            case 2:
                                Compute_product_col_by_col(startIdx, endIdx, threadNum);
                                break;
                            case 3:
                                Compute_product_every_k(threadNum, thread_count, (matrixSize * matrixSize) / thread_count, threadNum);
                                break;
                        }
                    });

                    tasks.Add(t);
                }

                tasks.ForEach(t => t.Start());

                Task.WaitAll([.. tasks]);

                stopwatch.Stop();
                Console.WriteLine($"Elapsed time: {stopwatch.ElapsedMilliseconds} ms");

                Console.WriteLine("\nMatrix A:");
                for (int i = 0; i < matrixSize; i++)
                {
                    for (int j = 0; j < matrixSize; j++)
                    {
                        Console.Write(matrixA![i, j] + " ");
                    }
                    Console.WriteLine();
                }

                Console.WriteLine("Matrix B:");
                for (int i = 0; i < matrixSize; i++)
                {
                    for (int j = 0; j < matrixSize; j++)
                    {
                        Console.Write(matrixB![i, j] + " ");
                    }
                    Console.WriteLine();
                }

                Console.WriteLine("Result Matrix:");
                for (int i = 0; i < matrixSize; i++)
                {
                    for (int j = 0; j < matrixSize; j++)
                    {
                        Console.Write(resultMatrix![i, j] + " ");
                    }
                    Console.WriteLine();
                }
                Console.WriteLine("\n");
            }
        }
    }
}