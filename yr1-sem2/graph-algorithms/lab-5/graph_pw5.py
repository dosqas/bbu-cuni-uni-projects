import random
import copy
ok = False


def for_tests():
    from graph_pw5_tests import Testing
    import unittest

    suite = unittest.TestLoader().loadTestsFromTestCase(Testing)
    unittest.TextTestRunner().run(suite)


def load_from_file(filename):
    global ok
    try:
        file = open(filename, "r")

        if file is None:
            print("File is nonexistent.")
            ok = False
            return
    except FileNotFoundError:
        print("File is nonexistent.")
        ok = False
        return

    nr_vertices, nr_edges = map(int, file.readline().split())
    graph = Graph(nr_vertices)
    for _ in range(nr_edges):
        src, dst, cos = map(int, file.readline().split())
        graph.add_edge(src, dst, cos)

    file.close()
    return graph


def save_to_file(filename, graph):
    file = open(filename, "w")

    file.write("{} {}\n".format(graph.count_vertices(), graph.count_edges()))

    for src in graph.vert_iter():
        for dst in graph.destination_iter(src):
            file.write("{} {} {}\n".format(src, dst, graph.get_cost(src, dst)))

    file.close()


def random_graph_generator(nr_vertices, nr_edges):
    graph = Graph(nr_vertices)
    for _ in range(0, nr_edges, 1):
        test = False
        while test is False:
            src = random.randint(0, nr_vertices)
            dst = random.randint(0, nr_vertices)
            try:
                while graph.is_an_edge(src, dst):
                    src = random.randint(0, nr_vertices)
                    dst = random.randint(0, nr_vertices)
                cost = random.randint(0, 100)
                graph.add_edge(src, dst, cost)
                test = True
            except ValueError:
                pass
    return graph


class Graph:
    def __init__(self, nr_vertices):
        self.vertices = set()
        self.costs = {}
        self.outbound = {}
        self.inbound = {}

        for cnt in range(0, nr_vertices, 1):
            self.add_vertex(cnt)

    def vert_iter(self):
        for ver in self.vertices:
            yield ver

    def is_vertex(self, ver):
        return ver in self.vertices

    def count_vertices(self):
        return len(self.vertices)

    def edge_iter(self):
        for srcANDdest, cost in self.costs.items():
            yield srcANDdest[0], srcANDdest[1], cost

    def is_an_edge(self, src, dst):
        return src in self.outbound and dst in self.outbound[src]

    def count_edges(self):
        return len(self.costs)

    def copy(self):
        return copy.deepcopy(self)

    def source_iter(self, ver):
        if not self.is_vertex(ver):
            raise ValueError("Vertex is nonexistent.")

        for src in self.inbound[ver]:
            yield src

    def destination_iter(self, ver):
        if not self.is_vertex(ver):
            raise ValueError("Vertex is nonexistent.")

        for dst in self.outbound[ver]:
            yield dst

    def degree_inbound(self, ver):
        if ver not in self.inbound:
            raise ValueError("Vertex is nonexistent.")

        return len(self.inbound[ver])

    def degree_outbound(self, ver):
        if ver not in self.outbound:
            raise ValueError("Vertex is nonexistent.")

        return len(self.outbound[ver])

    def get_cost(self, src, dst):
        if (src, dst) not in self.costs:
            raise ValueError("Edge is nonexistent.")

        return self.costs[(src, dst)]

    def set_cost(self, src, dst, cost):
        if (src, dst) not in self.costs:
            raise ValueError("Edge is nonexistent.")

        self.costs[(src, dst)] = cost

    def add_vertex(self, ver):
        if self.is_vertex(ver):
            raise ValueError("Vertex already exists.")

        self.vertices.add(ver)
        self.outbound[ver] = set()
        self.inbound[ver] = set()

    def remove_vertex(self, ver):
        if not self.is_vertex(ver):
            raise ValueError("Vertex is nonexistent.")

        temp_connections = []
        for dst in self.outbound[ver]:
            temp_connections.append(dst)
        for dst in temp_connections:
            self.remove_edge(ver, dst)
        temp_connections = []
        for src in self.inbound[ver]:
            temp_connections.append(src)
        for src in temp_connections:
            self.remove_edge(src, ver)

        del self.outbound[ver]
        del self.inbound[ver]
        self.vertices.remove(ver)

    def add_edge(self, src, dst, cost):
        if self.is_an_edge(src, dst):
            raise ValueError("Edge already exists.")
        if not self.is_vertex(src) or not self.is_vertex(dst):
            raise ValueError("Source or destination vertex is nonexistent.")

        self.costs[(src, dst)] = cost
        self.outbound[src].add(dst)
        self.inbound[dst].add(src)

    def remove_edge(self, src, dst):
        if not self.is_an_edge(src, dst):
            raise ValueError("Edge is nonexistent.")

        del self.costs[(src, dst)]
        self.outbound[src].remove(dst)
        self.inbound[dst].remove(src)

    def minimum_vertex_coloring_aux(self):
        colors = {}
        for vertex in self.vert_iter():
            colors[vertex] = None

        def is_color_available(vert, color):
            for neighbor in self.inbound[vert]:
                if colors[neighbor] == color:
                    return False
            return True

        def get_lowest_available_color(vert):
            used_colors = set(colors[neighbor] for neighbor in self.inbound[vert] if colors[neighbor] is not None)
            for color in range(self.count_edges()):
                if color not in used_colors and is_color_available(vert, color):
                    return color

        # Color each vertex using the greedy algorithm
        for vertex in self.vert_iter():
            colors[vertex] = get_lowest_available_color(vertex)

        return colors


class UI:
    def __init__(self):
        self.graph = None
        self.graph_copy = None

    def start(self):
        while True:
            try:
                print("[MENU]\n"
                      "Choose a command:\n"
                      "[1] Vertex operation menu.\n"
                      "[2] Edge operation menu.\n"
                      "[3] Graph operation menu.\n"
                      "[4] Find minimum coloring of vertices.\n"
                      "[5] Exit.\n")
                command = input("Enter input: ")
                if not command.isdigit():
                    raise ValueError
                command = int(command)
                if (command == 1 or command == 2 or command == 4) and self.graph is None:
                    print("Try again after loading/creating a graph.\n")
                elif command == 1 and self.graph is not None:
                    self.vertex_menu()
                elif command == 2 and self.graph is not None:
                    self.edge_menu()
                elif command == 3:
                    self.graph_menu()
                elif command == 4:
                    self.minimum_vertex_coloring()
                elif command == 5:
                    print("Goodbye!")
                    quit()
                else:
                    print("Invalid command.\n")
            except ValueError:
                print("Invalid command.\n")

    def vertex_menu(self):
        try:
            while True:
                print("\n[VERTEX OP MENU]\n"
                      "Choose a command:\n"
                      "[1] Get number of vertices.\n"
                      "[2] Parse the set of vertices.\n"
                      "[3] Retrieve the in/out degree of a vertex.\n"
                      "[4] Add a vertex.\n"
                      "[5] Remove a vertex.\n"
                      "[6] Back.\n")
                command = input("Enter input: ")
                if not command.isdigit():
                    raise ValueError
                command = int(command)
                if command == 1:
                    print("Number of vertices: {}\n".format(self.graph.count_vertices()))
                elif command == 2:
                    print("Set of vertices: {}\n".format(list(self.graph.vert_iter())))
                elif command == 3:
                    vertex = int(input("Enter vertex: "))
                    print("In degree: {}\nOut degree: {}\n".format(self.graph.degree_inbound(vertex),
                                                                   self.graph.degree_outbound(vertex)))
                elif command == 4:
                    vertex = int(input("Enter vertex: "))
                    self.graph.add_vertex(vertex)
                elif command == 5:
                    vertex = int(input("Enter vertex: "))
                    self.graph.remove_vertex(vertex)
                elif command == 6:
                    break
                else:
                    print("Invalid command.\n")
                print("Success.\n")
        except ValueError:
            print("Invalid command.\n")

    def edge_menu(self):
        try:
            while True:
                print("\n[EDGE OP MENU]\n"
                      "Choose a command:\n"
                      "[1] Check if an edge between two vertices exists.\n"
                      "[2] Parse the set of outbound edges of a vertex.\n"
                      "[3] Parse the set of inbound edges of a vertex.\n"
                      "[4] Retrieve the cost of an edge.\n"
                      "[5] Modify the cost of an edge.\n"
                      "[6] Add an edge.\n"
                      "[7] Remove an edge.\n"
                      "[8] Back.\n")
                command = input("Enter input: ")
                if not command.isdigit():
                    raise ValueError
                command = int(command)
                if command == 1:
                    src = int(input("Enter source vertex: "))
                    dst = int(input("Enter destination vertex: "))
                    print("Edge exists: {}\n".format(self.graph.is_an_edge(src, dst)))
                elif command == 2:
                    vertex = int(input("Enter vertex: "))
                    print("Set of outbound edges: {}\n".format(list(self.graph.source_iter(vertex))))
                    # outbound edges = edgeurile ce au ca sursa vertexu original adica de la vertex la restu si da setu ala
                elif command == 3:
                    vertex = int(input("Enter vertex: "))
                    print("Set of inbound edges: {}\n".format(list(self.graph.destination_iter(vertex))))
                elif command == 4:
                    src = int(input("Enter source vertex: "))
                    dst = int(input("Enter destination vertex: "))
                    print("Cost: {}\n".format(self.graph.get_cost(src, dst)))
                elif command == 5:
                    src = int(input("Enter source vertex: "))
                    dst = int(input("Enter destination vertex: "))
                    cost = int(input("Enter cost: "))
                    self.graph.set_cost(src, dst, cost)
                elif command == 6:
                    src = int(input("Enter source vertex: "))
                    dst = int(input("Enter destination vertex: "))
                    cost = int(input("Enter cost: "))
                    self.graph.add_edge(src, dst, cost)
                elif command == 7:
                    src = int(input("Enter source vertex: "))
                    dst = int(input("Enter destination vertex: "))
                    self.graph.remove_edge(src, dst)
                elif command == 8:
                    break
                else:
                    print("Invalid command.\n")
                print("Success.\n")
        except ValueError:
            print("Invalid command.\n")

    def graph_menu(self):
        global ok
        try:
            while True:
                ok = True
                print("\n[GRAPH OP MENU]\n"
                      "Choose a command:\n"
                      "[1] Read the graph from a text file.\n"
                      "[2] Write the graph to a text file.\n"
                      "[3] Create a random graph.\n"
                      "[4] Copy the graph (saving the state).\n"
                      "[5] Load the copy\n"
                      "[6] Back.\n")
                command = input("Enter input: ")
                if not command.isdigit():
                    raise ValueError
                command = int(command)
                if command == 1:
                    filename = input("Enter filename: ")
                    self.graph = load_from_file(filename)
                elif command == 2:
                    filename = input("Enter filename: ")
                    save_to_file(filename, self.graph)
                elif command == 3:
                    nr_vertices = int(input("Enter number of vertices: "))
                    nr_edges = int(input("Enter number of edges: "))
                    if nr_edges > nr_vertices * (nr_vertices - 1):
                        print("Too many edges. Created a graph with 0 vertices and edges.\n")
                        ok = False
                        self.graph = random_graph_generator(0, 0)
                    else:
                        self.graph = random_graph_generator(nr_vertices, nr_edges)
                elif command == 4:
                    if self.graph is not None:
                        self.graph_copy = self.graph.copy()
                    else:
                        print("No graph available.\n")
                        ok = False
                elif command == 5:
                    if self.graph_copy is not None:
                        self.graph = self.graph_copy
                    else:
                        print("No copy available.\n")
                        ok = False
                elif command == 6:
                    break
                else:
                    print("Invalid command.\n")
                if ok is True:
                    print("Success.\n")
        except ValueError:
            print("Invalid command.\n")

    def minimum_vertex_coloring(self):
        try:
            colors = self.graph.minimum_vertex_coloring_aux()
            print("Coloring: {}\n".format(colors))
        except ValueError as ve:
            print(ve)


if __name__ == "__main__":
    UI = UI()
    for_tests()
    UI.start()
