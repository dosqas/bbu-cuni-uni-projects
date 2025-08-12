import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.Random;
import java.util.Scanner;

public class UI {
    private Graph graph;
    private Graph graphCopy;

    public UI() {
        graph = null;
        graphCopy = null;
    }

    public void start() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            try {
                System.out.println("[MENU]");
                System.out.println("Choose a command:");
                System.out.println("[1] Vertex operation menu.");
                System.out.println("[2] Edge operation menu.");
                System.out.println("[3] Graph operation menu.");
                System.out.println("[4] Exit.");
                System.out.print("Enter input: ");
                int command = scanner.nextInt();
                if (command == 1 || command == 2) {
                    if (graph == null) {
                        System.out.println("Try again after loading/creating a graph.\n");
                        continue;
                    }
                }
                switch (command) {
                    case 1:
                        vertexMenu();
                        break;
                    case 2:
                        edgeMenu();
                        break;
                    case 3:
                        graphMenu();
                        break;
                    case 4:
                        System.out.println("Goodbye!");
                        System.exit(0);
                    default:
                        System.out.println("Invalid command.\n");
                }
            } catch (Exception e) {
                System.out.println("Invalid command.\n");
            }
        }
    }

    public void vertexMenu() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            try {
                System.out.println("\n[VERTEX OP MENU]");
                System.out.println("Choose a command:");
                System.out.println("[1] Get number of vertices.");
                System.out.println("[2] Parse the set of vertices.");
                System.out.println("[3] Retrieve the in/out degree of a vertex.");
                System.out.println("[4] Add a vertex.");
                System.out.println("[5] Remove a vertex.");
                System.out.println("[6] Back.");
                System.out.print("Enter input: ");
                int command = scanner.nextInt();
                switch (command) {
                    case 1:
                        System.out.println("Number of vertices: " + graph.countVertices() + "\n");
                        break;
                    case 2:
                        Iterator<Integer> vertices = graph.vertIter();
                        StringBuilder sb = new StringBuilder();
                        sb.append("[");
                        while (vertices.hasNext()) {
                            sb.append(vertices.next());
                            if (vertices.hasNext()) {
                                sb.append(", ");
                            }
                        }
                        sb.append("]");
                        System.out.println("Set of vertices: " + sb.toString() + "\n");
                        sb = new StringBuilder();
                        break;
                    case 3:
                        System.out.print("Enter vertex: ");
                        int vertex = scanner.nextInt();
                        System.out.println("In degree: " + graph.degreeInbound(vertex));
                        System.out.println("Out degree: " + graph.degreeOutbound(vertex) + "\n");
                        break;
                    case 4:
                        System.out.print("Enter vertex: ");
                        vertex = scanner.nextInt();
                        graph.addVertex(vertex);
                        break;
                    case 5:
                        System.out.print("Enter vertex: ");
                        vertex = scanner.nextInt();
                        graph.removeVertex(vertex);
                        break;
                    case 6:
                        return;
                    default:
                        System.out.println("Invalid command.\n");
                }
                System.out.println("Success.\n");
            } catch (Exception e) {
                System.out.println("Invalid command.\n");
            }
        }
    }

    public void edgeMenu() {
        Scanner scanner = new Scanner(System.in);
        int vertex;
        StringBuilder sb = new StringBuilder();

        while (true) {
            try {
                System.out.println("\n[EDGE OP MENU]");
                System.out.println("Choose a command:");
                System.out.println("[1] Check if an edge between two vertices exists.");
                System.out.println("[2] Parse the set of outbound edges of a vertex.");
                System.out.println("[3] Parse the set of inbound edges of a vertex.");
                System.out.println("[4] Retrieve the cost of an edge.");
                System.out.println("[5] Modify the cost of an edge.");
                System.out.println("[6] Add an edge.");
                System.out.println("[7] Remove an edge.");
                System.out.println("[8] Back.");
                System.out.print("Enter input: ");
                int command = scanner.nextInt();
                switch (command) {
                    case 1:
                        System.out.print("Enter source vertex: ");
                        int src = scanner.nextInt();
                        System.out.print("Enter destination vertex: ");
                        int dst = scanner.nextInt();
                        System.out.println("Edge exists: " + graph.isAnEdge(src, dst) + "\n");
                        break;
                    case 2:
                        System.out.print("Enter vertex: ");
                        vertex = scanner.nextInt();
                        Iterator<Integer> outboundEdges = graph.sourceIter(vertex);
                        sb.append("[");
                        while (outboundEdges.hasNext()) {
                            sb.append(outboundEdges.next());
                            if (outboundEdges.hasNext()) {
                                sb.append(", ");
                            }
                        }
                        sb.append("]");
                        System.out.println("Set of outbound edges: " + sb.toString() + "\n");
                        sb = new StringBuilder();
                        break;
                    case 3:
                        System.out.print("Enter vertex: ");
                        vertex = scanner.nextInt();
                        Iterator<Integer> inboundEdges = graph.destinationIter(vertex);
                        sb.append("[");
                        while (inboundEdges.hasNext()) {
                            sb.append(inboundEdges.next());
                            if (inboundEdges.hasNext()) {
                                sb.append(", ");
                            }
                        }
                        sb.append("]");
                        System.out.println("Set of inbound edges: " + sb.toString() + "\n");
                        sb = new StringBuilder();
                        break;
                    case 4:
                        System.out.print("Enter source vertex: ");
                        src = scanner.nextInt();
                        System.out.print("Enter destination vertex: ");
                        dst = scanner.nextInt();
                        System.out.println("Cost: " + graph.getCost(src, dst) + "\n");
                        break;
                    case 5:
                        System.out.print("Enter source vertex: ");
                        src = scanner.nextInt();
                        System.out.print("Enter destination vertex: ");
                        dst = scanner.nextInt();
                        System.out.print("Enter cost: ");
                        int cost = scanner.nextInt();
                        graph.setCost(src, dst, cost);
                        break;
                    case 6:
                        System.out.print("Enter source vertex: ");
                        src = scanner.nextInt();
                        System.out.print("Enter destination vertex: ");
                        dst = scanner.nextInt();
                        System.out.print("Enter cost: ");
                        cost = scanner.nextInt();
                        graph.addEdge(src, dst, cost);
                        break;
                    case 7:
                        System.out.print("Enter source vertex: ");
                        src = scanner.nextInt();
                        System.out.print("Enter destination vertex: ");
                        dst = scanner.nextInt();
                        graph.removeEdge(src, dst);
                        break;
                    case 8:
                        return;
                    default:
                        System.out.println("Invalid command.\n");
                }
                System.out.println("Success.\n");
            } catch (Exception e) {
                System.out.println("Invalid command.\n");
            }
        }
    }

    public void graphMenu() {
        Scanner scanner = new Scanner(System.in);
        boolean ok = true;
        while (true) {
            try {
                ok = true;
                System.out.println("\n[GRAPH OP MENU]");
                System.out.println("Choose a command:");
                System.out.println("[1] Read the graph from a text file.");
                System.out.println("[2] Write the graph to a text file.");
                System.out.println("[3] Create a random graph.");
                System.out.println("[4] Copy the graph (saving the state).");
                System.out.println("[5] Load the copy.");
                System.out.println("[6] Back.");
                System.out.print("Enter input: ");
                int command = scanner.nextInt();
                switch (command) {
                    case 1:
                        System.out.print("Enter filename: ");
                        String filename = scanner.next();
                        graph = loadFromFile(filename);
                        break;
                    case 2:
                        System.out.print("Enter filename: ");
                        filename = scanner.next();
                        saveToFile(filename, graph);
                        break;
                    case 3:
                        System.out.print("Enter number of vertices: ");
                        int nr_vertices = scanner.nextInt();
                        System.out.print("Enter number of edges: ");
                        int nr_edges = scanner.nextInt();
                        if (nr_edges > nr_vertices * (nr_vertices - 1)) {
                            System.out.println("Too many edges. Created a graph with 0 vertices and edges.\n");
                            ok = false;
                            graph = randomGraphGenerator(0, 0);
                        } else {
                            graph = randomGraphGenerator(nr_vertices, nr_edges);
                        }
                        break;
                    case 4:
                        if (graph != null) {
                            graphCopy = graph.copy();
                        } else {
                            System.out.println("No graph available.\n");
                            ok = false;
                        }
                        break;
                    case 5:
                        if (graphCopy != null) {
                            graph = graphCopy;
                        } else {
                            System.out.println("No copy available.\n");
                            ok = false;
                        }
                        break;
                    case 6:
                        return;
                    default:
                        System.out.println("Invalid command.\n");
                }
                if (ok) {
                    System.out.println("Success.\n");
                }
            } catch (Exception e) {
                System.out.println("Invalid command.\n");
            }
        }
    }

    public Graph loadFromFile(String filename) throws IOException {
        File file = new File("resources/" + filename);
        if (!file.exists()) {
            System.out.println("File is nonexistent.");
            return null;
        }
        Scanner scanner = new Scanner(file);
        int nr_vertices = scanner.nextInt();
        int nr_edges = scanner.nextInt();
        Graph graph = new Graph(nr_vertices);
        for (int i = 0; i < nr_edges; i++) {
            int src = scanner.nextInt();
            int dst = scanner.nextInt();
            int cost = scanner.nextInt();
            graph.addEdge(src, dst, cost);
        }
        scanner.close();
        return graph;
    }

    public void saveToFile(String filename, Graph graph) throws IOException {
        FileWriter writer = new FileWriter(filename);
        writer.write(graph.countVertices() + " " + graph.countEdges() + "\n");
        for (Iterator<Integer> it = graph.vertIter(); it.hasNext(); ) {
            int src = it.next();
            for (Iterator<Integer> iter = graph.destinationIter(src); iter.hasNext(); ) {
                int dst = iter.next();
                writer.write(src + " " + dst + " " + graph.getCost(src, dst) + "\n");
            }
        }
        writer.close();
    }

    public Graph randomGraphGenerator(int nr_vertices, int nr_edges) {
        Graph graph = new Graph(nr_vertices);
        Random random = new Random();
        for (int i = 0; i < nr_edges; i++) {
            boolean test = false;
            while (!test) {
                int src = random.nextInt(nr_vertices);
                int dst = random.nextInt(nr_vertices);
                try {
                    while (graph.isAnEdge(src, dst)) {
                        src = random.nextInt(nr_vertices);
                        dst = random.nextInt(nr_vertices);
                    }
                    int cost = random.nextInt(101);
                    graph.addEdge(src, dst, cost);
                    test = true;
                } catch (IllegalArgumentException e) {
                    // Do nothing
                }
            }
        }
        return graph;
    }

    public static void main(String[] args) {
        UI ui = new UI();
        ui.start();
    }
}
