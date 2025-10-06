using System.Numerics;
using System.Runtime.Intrinsics;

namespace lab_2;

class Program
{
    static readonly int vectorSize = 1_000_000;
    static int queueSize;
    static Queue<int> productQueue = new(queueSize);
    static readonly object queueLock = new object();
    static BigInteger vectorScalarProduct = 0;

    static void Main()
    {
        Console.WriteLine("Generating vectors...");
        List<int> vector1 = CreateRandomVector(vectorSize, 1);
        List<int> vector2 = CreateRandomVector(vectorSize, 2);
        Console.WriteLine("Vectors generated!\n\n");

        for (int i = 1; i <= 5; i++)
        {
            queueSize = (int)Math.Pow(10, i);
            productQueue = new Queue<int>(queueSize);

            Console.WriteLine("!! Starting timer");
            Console.WriteLine($"[Queue size: {queueSize}]");

            var watch = System.Diagnostics.Stopwatch.StartNew();
            vectorScalarProduct = 0;

            var p = Task.Run(() =>
            {
                StartProducer(vector1, vector2);
            });

            var c = Task.Run(() =>
            {
                StartConsumer();
            });

            Task.WaitAll(p, c);

            Console.WriteLine($"Final scalar product: {vectorScalarProduct}");
            watch.Stop();
            var elapsedMs = watch.ElapsedMilliseconds;
            Console.WriteLine($"Run {i}: {elapsedMs} ms\n\n");
        }
    }

    static List<int> CreateRandomVector(int size, int nr)
    {
        List<int> vector = new(size);

        for (int i = 0; i < size; i++)
        {
            // vector.Add(new Random().Next(0, 100));
            vector.Add(nr);
        }

        return vector;
    }

    static void StartProducer(List<int> vector1, List<int> vector2)
    {
        for (int i = 0; i < vectorSize; i++)
        {
            int product = vector1[i] * vector2[i];
            lock (queueLock)
            {
                while (productQueue.Count >= queueSize)
                {
                    Monitor.Wait(queueLock);
                }
                productQueue.Enqueue(product);
                Monitor.PulseAll(queueLock);
            }
        }
    }

    static void StartConsumer()
    {
        int itemsConsumed = 0;
        while (itemsConsumed < vectorSize)
        {
            int product;
            lock (queueLock)
            {
                while (productQueue.Count == 0)
                {
                    Monitor.Wait(queueLock);
                }
                product = productQueue.Dequeue();
                Monitor.PulseAll(queueLock);
            }
            vectorScalarProduct += product;
            itemsConsumed++;
        }
    }
}