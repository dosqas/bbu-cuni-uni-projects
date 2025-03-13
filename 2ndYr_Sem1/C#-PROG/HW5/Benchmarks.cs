using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Running;
using IncrementWordCountInDictionary;

BenchmarkRunner.Run<DataStructureBenchmark>();

namespace IncrementWordCountInDictionary
{
    /* [SCENARIO 1]
        ```

        BenchmarkDotNet v0.14.0, Windows 11 (10.0.22631.4460/23H2/2023Update/SunValley3)
        13th Gen Intel Core i5-13500H, 1 CPU, 16 logical and 12 physical cores
        .NET SDK 9.0.100
          [Host]     : .NET 7.0.20 (7.0.2024.26716), X64 RyuJIT AVX2
          DefaultJob : .NET 7.0.20 (7.0.2024.26716), X64 RyuJIT AVX2


        ```
        | Method                          | Mean     | Error    | StdDev   |
        |-------------------------------- |---------:|---------:|---------:|
        | Benchmark_IncrementWordCount_V1 | 49.97 ns | 0.110 ns | 0.103 ns |
        | Benchmark_IncrementWordCount_V2 | 68.25 ns | 0.118 ns | 0.105 ns |
        | Benchmark_IncrementWordCount_V3 | 49.20 ns | 0.156 ns | 0.146 ns |


        As we can see from the above copied report, IncrementWordCount_V1 and IncrementWordCount_V3 have basically the same 
        execution time(the small difference is negligible, caused by precision errors since the measured time is in nanoseconds). 
        IncrementWordCount_V2 is the slowest, being roughly around ~40% slower than the aforementioned methods.

        The second method is the slowest because we use both an if-else block, which slows down the execution speed due to the 
        introduction of branching which leads to branch misprediction and slow execution, and also because we use the ContainsKey 
        method, which uses a method to calculate the hash value of the key, then needing to locate the correct bucket in the hash 
        table, followed by actually searching the bucket for the specified key.We also first do a check and only then we actually 
        update the value of the key.This all leads to using unnecessary overhead

        The first method relies on using a try-catch block.Throwing and catching exceptions is quite costly, but in this context 
        it is quite simple and straight forward, not needing to call other methods or introducing branching, and as such leads to 
        a better time than the second version of incrementing. Another bonus would be that, if most of the keys used in the hash 
        table are already in it, we need not throw any exception, as such leading to an even better execution overall compared 
        to V2.

        What is curious though is that the third method has better complexity than the second one, even thought it uses a 
        TryGetValue method, which is very similar to the ContainsKey method in the sense that it also calls basically the same 
        methods. The difference here is though that there is no if-else that introduces branching, which in turns speeds our 
        execution considerably. 
    */
    public class DictionaryAddBenchmarks
    {
        Dictionary<string, int> d = new Dictionary<string, int>();
        [Benchmark]
        public void Benchmark_IncrementWordCount_V1()
        {
            Program.IncrementWordCount_V1(d, "one");
            Program.IncrementWordCount_V1(d, "one");
            Program.IncrementWordCount_V1(d, "two");
        }
        [Benchmark]
        public void Benchmark_IncrementWordCount_V2()
        {
            Program.IncrementWordCount_V2(d, "one");
            Program.IncrementWordCount_V2(d, "one");
            Program.IncrementWordCount_V2(d, "two");
        }

        [Benchmark]
        public void Benchmark_IncrementWordCount_V3()
        {
            Program.IncrementWordCount_V3(d, "one");
            Program.IncrementWordCount_V3(d, "one");

            Program.IncrementWordCount_V3(d, "two");

        }
    }


    /* [SCENARIO 2]
        ```

        BenchmarkDotNet v0.14.0, Windows 11 (10.0.22631.4460/23H2/2023Update/SunValley3)
        13th Gen Intel Core i5-13500H, 1 CPU, 16 logical and 12 physical cores
        .NET SDK 9.0.100
          [Host]     : .NET 7.0.20 (7.0.2024.26716), X64 RyuJIT AVX2
          DefaultJob : .NET 7.0.20 (7.0.2024.26716), X64 RyuJIT AVX2


        ```
        | Method                       | Mean     | Error   | StdDev  |
        |----------------------------- |---------:|--------:|--------:|
        | Benchmark_SortedList         | 149.2 ns | 0.36 ns | 0.34 ns |
        | Benchmark_SortedDictionary   | 108.5 ns | 0.20 ns | 0.18 ns |
        | Benchmark_DictionaryThenSort | 223.3 ns | 3.60 ns | 3.01 ns |


        For this scenario, the IncrementWordCount_V3 method was used to test the performance of these three data structures(I could 
        have also went forward with using V1, it would not have made a difference in these cases).

        By far, using a dictionary and then at the end sorting it was the slowest of the bunch.This is caused by it needing to 
        support all the regular dictionary operations(which are quite fast on their own), but on top of that, needing to sort it at 
        the end.That is a pretty slow process, since we need to go through the whole dictionary step by step, ordering by keys, 
        afterwards making it a dictionary again.OrderBy has a time complexity of O(N Log(N)), and converting the sorted collection 
        back to a dictionary afterwards adds an O(N) overhead, which negates the otherwise fast dictionary operations.

        Using a SortedList is ~30% less costly than the aforementioned method.This is mostly due to the fact that we keep the 
        underlying list sorted at all times, and as such we do not need to do very costly operations at the end, instead doing a 
        little costly operation at every step along the way. The internal list of keys is always kept sorted and when we insert a 
        new element we insert it at the correct spot in the key array by using binary search, then shifting elements to maintain 
        the sorted order(which is O(N) at worst). This leads to faster execution.

        Finally, using a SortedDictionary is the best. It is roughly ~30% faster than the SortedList and also >50% better than 
        using a Dictionary and then sorting it.This is caused due to the fact that the SortedDictionary uses a balanced binary 
        tree for it's internal representation, leading to overall more efficient executions compared to the SortedList(since we 
        do not need to shift any elements). It is also better than the first method we talked about since the sorting is ensured 
        with each insertion(which has complexity O(Log(N))) to be in the correct place. Even if the insertion itself is less 
        efficient than the insertion in a regular dictionary(which has complexity O(N) amortized), the fact that we do not need to
        sort makes it very good.
    */   
    public class DataStructureBenchmark
    {
        SortedList<string, int> sortedList = new SortedList<string, int>();
        SortedDictionary<string, int> sortedDictionary = new SortedDictionary<string, int>();
        Dictionary<string, int> dictionary = new Dictionary<string, int>();
        [Benchmark]
        public void Benchmark_SortedList()
        {
            Program.IncrementWordCount_V3(sortedList, "one");
            Program.IncrementWordCount_V3(sortedList, "one");
            Program.IncrementWordCount_V3(sortedList, "two");
        }
        [Benchmark]
        public void Benchmark_SortedDictionary()
        {
            Program.IncrementWordCount_V3(sortedDictionary, "one");
            Program.IncrementWordCount_V3(sortedDictionary, "one");
            Program.IncrementWordCount_V3(sortedDictionary, "two");
        }
        [Benchmark]
        public void Benchmark_DictionaryThenSort()
        {
            Program.IncrementWordCount_V3(dictionary, "one");
            Program.IncrementWordCount_V3(dictionary, "one");
            Program.IncrementWordCount_V3(dictionary, "two");

            var sorted = dictionary.OrderBy(x => x.Key).ToDictionary(x => x.Key, x => x.Value);
        }
    }



    /* [CONCLUSION]
        In conclusion, for the Word Frequency assignment using the IncrementWordCount_V1 alongside a SortedDictionary would lead 
        to the best results.

        I have chosen the V1 due to the fact that it can perform exceptionally well compared to V3 over a long enough execution 
        due to the fact that, while exception handling can be costly, it's impact is lessened over extended runs when most keys 
        are already present in the table, and as such avoiding exceptions as a whole. In such cases, updates of the frequencies 
        occur in O(1) time complexity, which can outperform V3, due to it consistently acquiring the overhead of TryGetValue

        The SortedDictionary is also the best of the bunch when it comes to choosing an appropriate data structure for our 
        internal representation as we have discussed above.It manages to maintain a sorted order with efficient insertions/lookups
        in O(Log(N)) due to using a balanced binary tree for it's internal representation.
    */
}
