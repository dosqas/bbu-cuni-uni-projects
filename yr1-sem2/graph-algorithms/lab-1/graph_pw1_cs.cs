using System;
using System.Collections.Generic;
using System.IO;

namespace Graph
{
    internal class Program
    {
        static void Main(string[] args)
        {
            UI ui = new UI();
            ui.Start();
        }
    }

    public class Graph
    {
        private HashSet<int> vertices;
        private Dictionary<Tuple<int, int>, int> costs;
        private Dictionary<int, HashSet<int>> outbound;
        private Dictionary<int, HashSet<int>> inbound;

        public Graph(int nr_vertices)
        {
            vertices = new HashSet<int>();
            costs = new Dictionary<Tuple<int, int>, int>();
            outbound = new Dictionary<int, HashSet<int>>();
            inbound = new Dictionary<int, HashSet<int>>();
            for (int cnt = 0; cnt < nr_vertices; cnt++)
            {
                AddVertex(cnt);
            }
        }

        public IEnumerable<int> VertIter()
        {
            foreach (int ver in vertices)
            {
                yield return ver;
            }
        }

        public bool IsVertex(int ver)
        {
            return vertices.Contains(ver);
        }

        public int CountVertices()
        {
            return vertices.Count;
        }

        public IEnumerable<Tuple<int, int, int>> EdgeIter()
        {
            foreach (var entry in costs)
            {
                yield return Tuple.Create(entry.Key.Item1, entry.Key.Item2, entry.Value);
            }
        }

        public bool IsAnEdge(int src, int dst)
        {
            return outbound.ContainsKey(src) && outbound[src].Contains(dst);
        }

        public int CountEdges()
        {
            return costs.Count;
        }

        public Graph Copy()
        {
            Graph copy = new Graph(0);
            copy.vertices = new HashSet<int>(vertices);
            copy.costs = new Dictionary<Tuple<int, int>, int>(costs);
            copy.outbound = new Dictionary<int, HashSet<int>>(outbound);
            copy.inbound = new Dictionary<int, HashSet<int>>(inbound);
            return copy;
        }

        public IEnumerable<int> SourceIter(int ver)
        {
            if (!IsVertex(ver))
            {
                throw new ArgumentException("Vertex is nonexistent.");
            }
            foreach (int src in inbound[ver])
            {
                yield return src;
            }
        }

        public IEnumerable<int> DestinationIter(int ver)
        {
            if (!IsVertex(ver))
            {
                throw new ArgumentException("Vertex is nonexistent.");
            }
            foreach (int dst in outbound[ver])
            {
                yield return dst;
            }
        }

        public int DegreeInbound(int ver)
        {
            if (!inbound.ContainsKey(ver))
            {
                throw new ArgumentException("Vertex is nonexistent.");
            }
            return inbound[ver].Count;
        }

        public int DegreeOutbound(int ver)
        {
            if (!outbound.ContainsKey(ver))
            {
                throw new ArgumentException("Vertex is nonexistent.");
            }
            return outbound[ver].Count;
        }

        public int GetCost(int src, int dst)
        {
            if (!costs.ContainsKey(Tuple.Create(src, dst)))
            {
                throw new ArgumentException("Edge is nonexistent.");
            }
            return costs[Tuple.Create(src, dst)];
        }

        public void SetCost(int src, int dst, int cost)
        {
            if (!costs.ContainsKey(Tuple.Create(src, dst)))
            {
                throw new ArgumentException("Edge is nonexistent.");
            }
            costs[Tuple.Create(src, dst)] = cost;
        }

        public void AddVertex(int ver)
        {
            if (IsVertex(ver))
            {
                throw new ArgumentException("Vertex already exists.");
            }
            vertices.Add(ver);
            outbound[ver] = new HashSet<int>();
            inbound[ver] = new HashSet<int>();
        }

        public void RemoveVertex(int ver)
        {
            if (!IsVertex(ver))
            {
                throw new ArgumentException("Vertex is nonexistent.");
            }
            List<int> temp_connections = new List<int>();
            foreach (int dst in outbound[ver])
            {
                temp_connections.Add(dst);
            }
            foreach (int dst in temp_connections)
            {
                RemoveEdge(ver, dst);
            }
            temp_connections = new List<int>();
            foreach (int src in inbound[ver])
            {
                temp_connections.Add(src);
            }
            foreach (int src in temp_connections)
            {
                RemoveEdge(src, ver);
            }
            outbound.Remove(ver);
            inbound.Remove(ver);
            vertices.Remove(ver);
        }

        public void AddEdge(int src, int dst, int cost)
        {
            if (IsAnEdge(src, dst))
            {
                throw new ArgumentException("Edge already exists.");
            }
            if (!IsVertex(src) || !IsVertex(dst))
            {
                throw new ArgumentException("Source or destination vertex is nonexistent.");
            }
            costs[Tuple.Create(src, dst)] = cost;
            outbound[src].Add(dst);
            inbound[dst].Add(src);
        }

        public void RemoveEdge(int src, int dst)
        {
            if (!IsAnEdge(src, dst))
            {
                throw new ArgumentException("Edge is nonexistent.");
            }
            costs.Remove(Tuple.Create(src, dst));
            outbound[src].Remove(dst);
            inbound[dst].Remove(src);
        }
    }

    public class UI
    {
        private Graph graph;
        private Graph graphCopy;

        public void Start()
        {
            while (true)
            {
                try
                {
                    Console.WriteLine();
                    Console.WriteLine("[MENU]");
                    Console.WriteLine("Choose a command:");
                    Console.WriteLine("[1] Vertex operation menu.");
                    Console.WriteLine("[2] Edge operation menu.");
                    Console.WriteLine("[3] Graph operation menu.");
                    Console.WriteLine("[4] Exit.");
                    Console.Write("Enter input: ");
                    string command = Console.ReadLine();
                    if (!int.TryParse(command, out int cmd))
                    {
                        throw new ArgumentException();
                    }
                    if ((cmd == 1 || cmd == 2) && graph == null)
                    {
                        Console.WriteLine("Try again after loading/creating a graph.\n");
                    }
                    else if (cmd == 1 && graph != null)
                    {
                        VertexMenu();
                    }
                    else if (cmd == 2 && graph != null)
                    {
                        EdgeMenu();
                    }
                    else if (cmd == 3)
                    {
                        GraphMenu();
                    }
                    else if (cmd == 4)
                    {
                        Console.WriteLine("Goodbye!");
                        Environment.Exit(0);
                    }
                    else
                    {
                        Console.WriteLine("Invalid command.\n");
                    }
                }
                catch (ArgumentException)
                {
                    Console.WriteLine("Invalid command.\n");
                }
            }
        }

        public void VertexMenu()
        {
            try
            {
                while (true)
                {
                    Console.WriteLine("\n[VERTEX OP MENU]");
                    Console.WriteLine("Choose a command:");
                    Console.WriteLine("[1] Get number of vertices.");
                    Console.WriteLine("[2] Parse the set of vertices.");
                    Console.WriteLine("[3] Retrieve the in/out degree of a vertex.");
                    Console.WriteLine("[4] Add a vertex.");
                    Console.WriteLine("[5] Remove a vertex.");
                    Console.WriteLine("[6] Back.");
                    Console.Write("Enter input: ");
                    string command = Console.ReadLine();
                    if (!int.TryParse(command, out int cmd))
                    {
                        throw new ArgumentException();
                    }
                    if (cmd == 1)
                    {
                        Console.WriteLine("Number of vertices: {0}\n", graph.CountVertices());
                    }
                    else if (cmd == 2)
                    {
                        Console.WriteLine("Set of vertices: {0}\n", string.Join(", ", graph.VertIter()));
                    }
                    else if (cmd == 3)
                    {
                        Console.Write("Enter vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int vertex))
                        {
                            throw new ArgumentException();
                        }
                        Console.WriteLine("In degree: {0}\nOut degree: {1}\n", graph.DegreeInbound(vertex), graph.DegreeOutbound(vertex));
                    }
                    else if (cmd == 4)
                    {
                        Console.Write("Enter vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int vertex))
                        {
                            throw new ArgumentException();
                        }
                        graph.AddVertex(vertex);
                    }
                    else if (cmd == 5)
                    {
                        Console.Write("Enter vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int vertex))
                        {
                            throw new ArgumentException();
                        }
                        graph.RemoveVertex(vertex);
                    }
                    else if (cmd == 6)
                    {
                        break;
                    }
                    else
                    {
                        Console.WriteLine("Invalid command.\n");
                    }
                    Console.WriteLine("Success.\n");
                }
            }
            catch (ArgumentException)
            {
                Console.WriteLine("Invalid command.\n");
            }
        }

        public void EdgeMenu()
        {
            try
            {
                while (true)
                {
                    Console.WriteLine("\n[EDGE OP MENU]");
                    Console.WriteLine("Choose a command:");
                    Console.WriteLine("[1] Check if an edge between two vertices exists.");
                    Console.WriteLine("[2] Parse the set of outbound edges of a vertex.");
                    Console.WriteLine("[3] Parse the set of inbound edges of a vertex.");
                    Console.WriteLine("[4] Retrieve the cost of an edge.");
                    Console.WriteLine("[5] Modify the cost of an edge.");
                    Console.WriteLine("[6] Add an edge.");
                    Console.WriteLine("[7] Remove an edge.");
                    Console.WriteLine("[8] Back.");
                    Console.Write("Enter input: ");
                    string command = Console.ReadLine();
                    if (!int.TryParse(command, out int cmd))
                    {
                        throw new ArgumentException();
                    }
                    if (cmd == 1)
                    {
                        Console.Write("Enter source vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int src))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter destination vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int dst))
                        {
                            throw new ArgumentException();
                        }
                        Console.WriteLine("Edge exists: {0}\n", graph.IsAnEdge(src, dst));
                    }
                    else if (cmd == 2)
                    {
                        Console.Write("Enter vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int vertex))
                        {
                            throw new ArgumentException();
                        }
                        Console.WriteLine("Set of outbound edges: {0}\n", string.Join(", ", graph.SourceIter(vertex)));
                    }
                    else if (cmd == 3)
                    {
                        Console.Write("Enter vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int vertex))
                        {
                            throw new ArgumentException();
                        }
                        Console.WriteLine("Set of inbound edges: {0}\n", string.Join(", ", graph.DestinationIter(vertex)));
                    }
                    else if (cmd == 4)
                    {
                        Console.Write("Enter source vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int src))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter destination vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int dst))
                        {
                            throw new ArgumentException();
                        }
                        Console.WriteLine("Cost: {0}\n", graph.GetCost(src, dst));
                    }
                    else if (cmd == 5)
                    {
                        Console.Write("Enter source vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int src))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter destination vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int dst))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter cost: ");
                        if (!int.TryParse(Console.ReadLine(), out int cost))
                        {
                            throw new ArgumentException();
                        }
                        graph.SetCost(src, dst, cost);
                    }
                    else if (cmd == 6)
                    {
                        Console.Write("Enter source vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int src))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter destination vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int dst))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter cost: ");
                        if (!int.TryParse(Console.ReadLine(), out int cost))
                        {
                            throw new ArgumentException();
                        }
                        graph.AddEdge(src, dst, cost);
                    }
                    else if (cmd == 7)
                    {
                        Console.Write("Enter source vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int src))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter destination vertex: ");
                        if (!int.TryParse(Console.ReadLine(), out int dst))
                        {
                            throw new ArgumentException();
                        }
                        graph.RemoveEdge(src, dst);
                    }
                    else if (cmd == 8)
                    {
                        break;
                    }
                    else
                    {
                        Console.WriteLine("Invalid command.\n");
                    }
                    Console.WriteLine("Success.\n");
                }
            }
            catch (ArgumentException)
            {
                Console.WriteLine("Invalid command.\n");
            }
        }

        public void GraphMenu()
        {
            bool ok = false;
            try
            {
                while (true)
                {
                    ok = true;
                    Console.WriteLine("\n[GRAPH OP MENU]");
                    Console.WriteLine("Choose a command:");
                    Console.WriteLine("[1] Read the graph from a text file.");
                    Console.WriteLine("[2] Write the graph to a text file.");
                    Console.WriteLine("[3] Create a random graph.");
                    Console.WriteLine("[4] Copy the graph (saving the state).");
                    Console.WriteLine("[5] Load the copy.");
                    Console.WriteLine("[6] Back.");
                    Console.Write("Enter input: ");
                    string command = Console.ReadLine();
                    if (!int.TryParse(command, out int cmd))
                    {
                        throw new ArgumentException();
                    }
                    if (cmd == 1)
                    {
                        Console.Write("Enter filename: ");
                        string filename = Console.ReadLine();
                        graph = LoadFromFile(filename);
                    }
                    else if (cmd == 2)
                    {
                        Console.Write("Enter filename: ");
                        string filename = Console.ReadLine();
                        SaveToFile(filename, graph);
                    }
                    else if (cmd == 3)
                    {
                        Console.Write("Enter number of vertices: ");
                        if (!int.TryParse(Console.ReadLine(), out int nr_vertices))
                        {
                            throw new ArgumentException();
                        }
                        Console.Write("Enter number of edges: ");
                        if (!int.TryParse(Console.ReadLine(), out int nr_edges))
                        {
                            throw new ArgumentException();
                        }
                        if (nr_edges > nr_vertices * (nr_vertices - 1))
                        {
                            Console.WriteLine("Too many edges. Created a graph with 0 vertices and edges.\n");
                            ok = false;
                            graph = RandomGraphGenerator(0, 0);
                        }
                        else
                        {
                            graph = RandomGraphGenerator(nr_vertices, nr_edges);
                        }
                    }
                    else if (cmd == 4)
                    {
                        if (graph != null)
                        {
                            graphCopy = graph.Copy();
                        }
                        else
                        {
                            Console.WriteLine("No graph available.\n");
                            ok = false;
                        }
                    }
                    else if (cmd == 5)
                    {
                        if (graphCopy != null)
                        {
                            graph = graphCopy;
                        }
                        else
                        {
                            Console.WriteLine("No copy available.\n");
                            ok = false;
                        }
                    }
                    else if (cmd == 6)
                    {
                        break;
                    }
                    else
                    {
                        Console.WriteLine("Invalid command.\n");
                    }
                    if (ok)
                    {
                        Console.WriteLine("Success.\n");
                    }
                }
            }
            catch (ArgumentException)
            {
                Console.WriteLine("Invalid command.\n");
            }
        }

        public Graph LoadFromFile(string filename)
        {
            bool ok = false;
            try
            {
                filename = filename.Trim();
                StreamReader file = new StreamReader(filename);
                if (file == null)
                {
                    Console.WriteLine("File is nonexistent.");
                    ok = false;
                    return null;
                }
                string[] line = file.ReadLine().Split();
                int nr_vertices = int.Parse(line[0]);
                int nr_edges = int.Parse(line[1]);
                Graph graph = new Graph(nr_vertices);
                for (int i = 0; i < nr_edges; i++)
                {
                    line = file.ReadLine().Split();
                    int src = int.Parse(line[0]);
                    int dst = int.Parse(line[1]);
                    int cost = int.Parse(line[2]);
                    graph.AddEdge(src, dst, cost);
                }
                file.Close();
                return graph;
            }
            catch (FileNotFoundException)
            {
                Console.WriteLine("File is nonexistent.");
                ok = false;
                return null;
            }
        }

        public void SaveToFile(string filename, Graph graph)
        {
            StreamWriter file = new StreamWriter(filename);
            file.WriteLine("{0} {1}", graph.CountVertices(), graph.CountEdges());
            foreach (var entry in graph.EdgeIter())
            {
                file.WriteLine("{0} {1} {2}", entry.Item1, entry.Item2, entry.Item3);
            }
            file.Close();
        }

        public Graph RandomGraphGenerator(int nr_vertices, int nr_edges)
        {
            Graph graph = new Graph(nr_vertices);
            Random random = new Random();
            for (int i = 0; i < nr_edges; i++)
            {
                bool test = false;
                while (!test)
                {
                    int src = random.Next(0, nr_vertices);
                    int dst = random.Next(0, nr_vertices);
                    try
                    {
                        while (graph.IsAnEdge(src, dst))
                        {
                            src = random.Next(0, nr_vertices);
                            dst = random.Next(0, nr_vertices);
                        }
                        int cost = random.Next(0, 100);
                        graph.AddEdge(src, dst, cost);
                        test = true;
                    }
                    catch (ArgumentException)
                    {
                        continue;
                    }
                }
            }
            return graph;
        }
    }
}


