#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <algorithm>
#include <stdexcept>

class Graph {
private:
    std::set<int> vertices;
    std::map<std::pair<int, int>, int> costs;
    std::map<int, std::set<int>> outbound;
    std::map<int, std::set<int>> inbound;

public:
    Graph(int nr_vertices) {
        for (int cnt = 0; cnt < nr_vertices; cnt++) {
            add_vertex(cnt);
        }
    }

    void add_vertex(int ver) {
        if (is_vertex(ver)) {
            throw std::invalid_argument("Vertex already exists.");
        }
        vertices.insert(ver);
        outbound[ver] = std::set<int>();
        inbound[ver] = std::set<int>();
    }

    void remove_vertex(int ver) {
        if (!is_vertex(ver)) {
            throw std::invalid_argument("Vertex is nonexistent.");
        }
        std::vector<int> temp_connections;
        for (int dst : outbound[ver]) {
            temp_connections.push_back(dst);
        }
        for (int dst : temp_connections) {
            remove_edge(ver, dst);
        }
        temp_connections.clear();
        for (int src : inbound[ver]) {
            temp_connections.push_back(src);
        }
        for (int src : temp_connections) {
            remove_edge(src, ver);
        }
        outbound.erase(ver);
        inbound.erase(ver);
        vertices.erase(ver);
    }

    void add_edge(int src, int dst, int cost) {
        if (is_an_edge(src, dst)) {
            throw std::invalid_argument("Edge already exists.");
        }
        if (!is_vertex(src) || !is_vertex(dst)) {
            throw std::invalid_argument("Source or destination vertex is nonexistent.");
        }
        costs[std::make_pair(src, dst)] = cost;
        outbound[src].insert(dst);
        inbound[dst].insert(src);
    }

    void remove_edge(int src, int dst) {
        if (!is_an_edge(src, dst)) {
            throw std::invalid_argument("Edge is nonexistent.");
        }
        costs.erase(std::make_pair(src, dst));
        outbound[src].erase(dst);
        inbound[dst].erase(src);
    }

    bool is_vertex(int ver) {
        return vertices.count(ver) > 0;
    }

    int count_vertices() {
        return vertices.size();
    }

    bool is_an_edge(int src, int dst) {
        return outbound.count(src) > 0 && outbound[src].count(dst) > 0;
    }

    int count_edges() {
        return costs.size();
    }

    int get_cost(int src, int dst) {
        if (!is_an_edge(src, dst)) {
            throw std::invalid_argument("Edge is nonexistent.");
        }
        return costs[std::make_pair(src, dst)];
    }

    void set_cost(int src, int dst, int cost) {
        if (!is_an_edge(src, dst)) {
            throw std::invalid_argument("Edge is nonexistent.");
        }
        costs[std::make_pair(src, dst)] = cost;
    }

    std::set<int> vert_iter() {
        return vertices;
    }

    std::set<int> source_iter(int ver) {
        if (!is_vertex(ver)) {
            throw std::invalid_argument("Vertex is nonexistent.");
        }
        return inbound[ver];
    }

    std::set<int> destination_iter(int ver) {
        if (!is_vertex(ver)) {
            throw std::invalid_argument("Vertex is nonexistent.");
        }
        return outbound[ver];
    }

    int degree_inbound(int ver) {
        if (inbound.count(ver) == 0) {
            throw std::invalid_argument("Vertex is nonexistent.");
        }
        return inbound[ver].size();
    }

    int degree_outbound(int ver) {
        if (outbound.count(ver) == 0) {
            throw std::invalid_argument("Vertex is nonexistent.");
        }
        return outbound[ver].size();
    }

    Graph copy() {
        Graph copy_graph(count_vertices());
        for (int src : vertices) {
            for (int dst : outbound[src]) {
                copy_graph.add_edge(src, dst, get_cost(src, dst));
            }
        }
        return copy_graph;
    }
};

Graph load_from_file(std::string filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::invalid_argument("File is nonexistent.");
    }
    int nr_vertices, nr_edges;
    file >> nr_vertices >> nr_edges;
    Graph graph(nr_vertices);
    for (int i = 0; i < nr_edges; i++) {
        int src, dst, cost;
        file >> src >> dst >> cost;
        graph.add_edge(src, dst, cost);
    }
    file.close();
    return graph;
}

void save_to_file(std::string filename, Graph graph) {
    std::ofstream file(filename);
    file << graph.count_vertices() << " " << graph.count_edges() << "\n";
    for (int src : graph.vert_iter()) {
        for (int dst : graph.destination_iter(src)) {
            file << src << " " << dst << " " << graph.get_cost(src, dst) << "\n";
        }
    }
    file.close();
}

Graph random_graph_generator(int nr_vertices, int nr_edges) {
    Graph graph(nr_vertices);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nr_vertices - 1);
    for (int i = 0; i < nr_edges; i++) {
        int src = dis(gen);
        int dst = dis(gen);
        while (graph.is_an_edge(src, dst)) {
            src = dis(gen);
            dst = dis(gen);
        }
        int cost = dis(gen) % 101;
        graph.add_edge(src, dst, cost);
    }
    return graph;
}

class UI {
private:
    Graph graph = Graph(0);
    Graph graph_copy = Graph(0);

public:
    UI() {}

    void start() {
        while (true) {
            std::cout << "[MENU]\n"
                << "Choose a command:\n"
                << "[1] Vertex operation menu.\n"
                << "[2] Edge operation menu.\n"
                << "[3] Graph operation menu.\n"
                << "[4] Exit.\n"
                << std::endl;
            int command;
            std::cin >> command;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            if (command == 1 && graph.count_vertices() == 0) {
                std::cout << "Try again after loading/creating a graph." << std::endl;
                continue;
            }
            if (command == 1) {
                vertex_menu();
            }
            else if (command == 2) {
                edge_menu();
            }
            else if (command == 3) {
                graph_menu();
            }
            else if (command == 4) {
                std::cout << "Goodbye!" << std::endl;
                break;
            }
            else {
                std::cout << "Invalid command." << std::endl;
            }
        }
    }

    void vertex_menu() {
        while (true) {
            std::cout << "\n[VERTEX OP MENU]\n"
                << "Choose a command:\n"
                << "[1] Get number of vertices.\n"
                << "[2] Parse the set of vertices.\n"
                << "[3] Retrieve the in/out degree of a vertex.\n"
                << "[4] Add a vertex.\n"
                << "[5] Remove a vertex.\n"
                << "[6] Back.\n"
                << std::endl;
            int command;
            std::cin >> command;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            if (command == 1) {
                std::cout << "Number of vertices: " << graph.count_vertices() << std::endl;
            }
            else if (command == 2) {
                std::cout << "Set of vertices: ";
                for (int ver : graph.vert_iter()) {
                    std::cout << ver << " ";
                }
                std::cout << std::endl;
            }
            else if (command == 3) {
                int vertex;
                std::cout << "Enter vertex: ";
                std::cin >> vertex;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid vertex." << std::endl;
                    continue;
                }
                std::cout << "In degree: " << graph.degree_inbound(vertex) << std::endl;
                std::cout << "Out degree: " << graph.degree_outbound(vertex) << std::endl;
            }
            else if (command == 4) {
                int vertex;
                std::cout << "Enter vertex: ";
                std::cin >> vertex;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid vertex." << std::endl;
                    continue;
                }
                try {
                    graph.add_vertex(vertex);
                }
                catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command == 5) {
                int vertex;
                std::cout << "Enter vertex: ";
                std::cin >> vertex;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid vertex." << std::endl;
                    continue;
                }
                try {
                    graph.remove_vertex(vertex);
                }
                catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command == 6) {
                break;
            }
            else {
                std::cout << "Invalid command." << std::endl;
            }
            std::cout << "Success." << std::endl;
        }
    }

    void edge_menu() {
        while (true) {
            std::cout << "\n[EDGE OP MENU]\n"
                << "Choose a command:\n"
                << "[1] Check if an edge between two vertices exists.\n"
                << "[2] Parse the set of outbound edges of a vertex.\n"
                << "[3] Parse the set of inbound edges of a vertex.\n"
                << "[4] Retrieve the cost of an edge.\n"
                << "[5] Modify the cost of an edge.\n"
                << "[6] Add an edge.\n"
                << "[7] Remove an edge.\n"
                << "[8] Back.\n"
                << std::endl;
            int command;
            std::cin >> command;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            if (command == 1) {
                int src, dst;
                std::cout << "Enter source vertex: ";
                std::cin >> src;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid source vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter destination vertex: ";
                std::cin >> dst;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid destination vertex." << std::endl;
                    continue;
                }
                std::cout << "Edge exists: " << graph.is_an_edge(src, dst) << std::endl;
            }
            else if (command == 2) {
                int vertex;
                std::cout << "Enter vertex: ";
                std::cin >> vertex;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid vertex." << std::endl;
                    continue;
                }
                std::cout << "Set of outbound edges: ";
                for (int src : graph.source_iter(vertex)) {
                    std::cout << src << " ";
                }
                std::cout << std::endl;
            }
            else if (command == 3) {
                int vertex;
                std::cout << "Enter vertex: ";
                std::cin >> vertex;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid vertex." << std::endl;
                    continue;
                }
                std::cout << "Set of inbound edges: ";
                for (int dst : graph.destination_iter(vertex)) {
                    std::cout << dst << " ";
                }
                std::cout << std::endl;
            }
            else if (command == 4) {
                int src, dst;
                std::cout << "Enter source vertex: ";
                std::cin >> src;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid source vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter destination vertex: ";
                std::cin >> dst;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid destination vertex." << std::endl;
                    continue;
                }
                std::cout << "Cost: " << graph.get_cost(src, dst) << std::endl;
            }
            else if (command == 5) {
                int src, dst, cost;
                std::cout << "Enter source vertex: ";
                std::cin >> src;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid source vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter destination vertex: ";
                std::cin >> dst;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid destination vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter cost: ";
                std::cin >> cost;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid cost." << std::endl;
                    continue;
                }
                graph.set_cost(src, dst, cost);
            }
            else if (command == 6) {
                int src, dst, cost;
                std::cout << "Enter source vertex: ";
                std::cin >> src;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid source vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter destination vertex: ";
                std::cin >> dst;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid destination vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter cost: ";
                std::cin >> cost;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid cost." << std::endl;
                    continue;
                }
                try {
                    graph.add_edge(src, dst, cost);
                }
                catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command == 7) {
                int src, dst;
                std::cout << "Enter source vertex: ";
                std::cin >> src;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid source vertex." << std::endl;
                    continue;
                }
                std::cout << "Enter destination vertex: ";
                std::cin >> dst;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid destination vertex." << std::endl;
                    continue;
                }
                try {
                    graph.remove_edge(src, dst);
                }
                catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command == 8) {
                break;
            }
            else {
                std::cout << "Invalid command." << std::endl;
            }
            std::cout << "Success." << std::endl;
        }
    }

    void graph_menu() {
        bool ok = true;
        while (true) {
            ok = true;
            std::cout << "\n[GRAPH OP MENU]\n"
                << "Choose a command:\n"
                << "[1] Read the graph from a text file.\n"
                << "[2] Write the graph to a text file.\n"
                << "[3] Create a random graph.\n"
                << "[4] Copy the graph (saving the state).\n"
                << "[5] Load the copy.\n"
                << "[6] Back.\n"
                << std::endl;
            int command;
            std::cin >> command;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid command." << std::endl;
                continue;
            }
            if (command == 1) {
                std::string filename;
                std::cout << "Enter filename: ";
                std::cin >> filename;
                try {
                    graph = load_from_file(filename);
                }
                catch (const std::invalid_argument& e) {
                    std::cout << e.what() << std::endl;
                }
            }
            else if (command == 2) {
                std::string filename;
                std::cout << "Enter filename: ";
                std::cin >> filename;
                save_to_file(filename, graph);
            }
            else if (command == 3) {
                int nr_vertices, nr_edges;
                std::cout << "Enter number of vertices: ";
                std::cin >> nr_vertices;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid number of vertices." << std::endl;
                    continue;
                }
                std::cout << "Enter number of edges: ";
                std::cin >> nr_edges;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid number of edges." << std::endl;
                    continue;
                }
                if (nr_edges > nr_vertices * (nr_vertices - 1)) {
                    std::cout << "Too many edges. Created a graph with 0 vertices and edges." << std::endl;
                    ok = false;
                    graph = random_graph_generator(0, 0);
                }
                else {
                    graph = random_graph_generator(nr_vertices, nr_edges);
                }
            }
            else if (command == 4) {
                if (graph.count_vertices() > 0) {
                    graph_copy = graph.copy();
                }
                else {
                    std::cout << "No graph available." << std::endl;
                    ok = false;
                }
            }
            else if (command == 5) {
                if (graph_copy.count_vertices() > 0) {
                    graph = graph_copy;
                }
                else {
                    std::cout << "No copy available." << std::endl;
                    ok = false;
                }
            }
            else if (command == 6) {
                break;
            }
            else {
                std::cout << "Invalid command." << std::endl;
            }
            if (ok) {
                std::cout << "Success." << std::endl;
            }
        }
    }
};

int main() {
    UI ui;
    ui.start();
    return 0;
}
