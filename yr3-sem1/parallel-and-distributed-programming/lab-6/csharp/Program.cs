class Program
{
    static readonly int graph_node_count = 10;
    static readonly int starting_node = 0;
    static readonly int thread_count = 8;
    static readonly List<int>[] graph = new List<int>[graph_node_count];

    static void GenerateGraph()
    {
        var rand = new Random(0);

        for (int i = 0; i < graph_node_count; i++)
        {
            graph[i] = [];
            for (int j = 0; j < graph_node_count; j++)
            {
                if (i != j && rand.Next(1, 4) == 1) // ~33% chance of edge
                    graph[i].Add(j);
            }
        }

        /*
        for (int i = 0; i < graph_node_count; i++)
        {
            Console.Write($"graph[{i}] = ");
            Console.WriteLine(string.Join(", ", graph[i]));
        }
        */
    }

    // Helper function to distribute threads evenly across branches
    static int[] DistributeThreads(int totalThreads, int branchCount)
    {
        int[] allocation = new int[branchCount];
        for (int i = 0; i < totalThreads; i++)
            allocation[i % branchCount]++;
        return allocation;
    }

    class Result
    {
        public bool Found;
        public List<int>? Path;
    }

    static void ParallelHamiltonianDFS(int current, bool[] visited, List<int> path, int start, int availableThreads, Result result)
    {
        // Volatile.Read ensures that we read the most up-to-date value from the main memory, not a cached version.
        if (Volatile.Read(ref result.Found)) return; // Stop if a cycle has already been found

        // If the path includes all nodes, check if there's an edge back to the start
        if (path.Count == graph_node_count)
        {
            // If there's an edge back to the start, we've found a Hamiltonian cycle
            if (graph[current].Contains(start))
            {
                // Use lock to ensure thread-safe update of the result
                lock (result)
                {
                    if (!result.Found)
                    {
                        result.Found = true;
                        result.Path = [.. path, start];
                    }
                }
            }
            return;
        }

        // Explore unvisited neighbors (a Hamiltonian path must visit each node exactly once)
        var neighbors = graph[current].Where(n => !visited[n]).ToList();
        if (neighbors.Count == 0)
            return;

        // If only one thread is available, proceed sequentially
        if (availableThreads <= 1)
        {
            foreach (var next in neighbors)
            {
                if (Volatile.Read(ref result.Found)) return;
                visited[next] = true;
                path.Add(next); // Add to path
                ParallelHamiltonianDFS(next, visited, path, start, 1, result);
                path.RemoveAt(path.Count - 1); // Backtrack - remove the node from path
                visited[next] = false;
            }
            return;
        }

        var threadAllocations = DistributeThreads(availableThreads, neighbors.Count);
        // Console.WriteLine($"Thread allocations for node {current} with neighbours {string.Join(", ", neighbors)}: {string.Join(", ", threadAllocations)}");
        var tasks = new List<Task>();

        for (int i = 0; i < neighbors.Count; i++)
        {
            int threadsForBranch = threadAllocations[i];
            int next = neighbors[i];

            // Clone visited array and path for the new task
            bool[] newVisited = (bool[])visited.Clone();
            newVisited[next] = true;
            var newPath = new List<int>(path) { next };

            tasks.Add(Task.Run(() =>
                ParallelHamiltonianDFS(next, newVisited, newPath, start, threadsForBranch, result)
            ));
        }

        Task.WaitAll([.. tasks]);
    }

    static void Main()
    {
        GenerateGraph();

        var visited = new bool[graph_node_count];
        var path = new List<int> { starting_node };
        visited[starting_node] = true;

        Console.WriteLine($"Starting parallel Hamiltonian cycle search with {thread_count} threads...");

        var result = new Result();
        ParallelHamiltonianDFS(starting_node, visited, path, starting_node, thread_count, result);

        if (result.Found && result.Path != null)
        {
            Console.WriteLine("\nHamiltonian cycle found:");
            Console.WriteLine(string.Join(" -> ", result.Path));
        }
        else
        {
            Console.WriteLine("\nNo Hamiltonian cycle exists.");
        }
    }
}
