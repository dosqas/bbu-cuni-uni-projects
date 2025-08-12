import unittest
from graph_pw4 import *


class Testing(unittest.TestCase):
    def setUp(self):
        self.graph = Graph(3)
        self.graph.add_edge(0, 1, 1)
        self.graph.add_edge(1, 2, 2)
        self.graph.add_edge(2, 0, 3)

    def test_vertex_iter(self):
        self.assertEqual(list(self.graph.vert_iter()), [0, 1, 2])

    def test_is_vertex(self):
        self.assertEqual(self.graph.is_vertex(0), True)
        self.assertEqual(self.graph.is_vertex(3), False)

    def test_count_vertices(self):
        self.assertEqual(self.graph.count_vertices(), 3)

    def test_edge_iter(self):
        self.assertEqual(list(self.graph.edge_iter()), [(0, 1, 1), (1, 2, 2), (2, 0, 3)])

    def test_is_an_edge(self):
        self.assertEqual(self.graph.is_an_edge(0, 1), True)
        self.assertEqual(self.graph.is_an_edge(1, 0), False)

    def test_count_edges(self):
        self.assertEqual(self.graph.count_edges(), 3)

    def test_copy(self):
        self.graph_copy = self.graph.copy()
        self.assertEqual(self.graph_copy.count_edges(), 3)

    def test_source_iter(self):
        self.assertEqual(list(self.graph.source_iter(2)), [1])
        self.assertEqual(list(self.graph.source_iter(0)), [2])

    def test_destination_iter(self):
        self.assertEqual(list(self.graph.destination_iter(0)), [1])
        self.assertEqual(list(self.graph.destination_iter(2)), [0])

    def test_degree_inbound(self):
        self.assertEqual(self.graph.degree_inbound(0), 1)
        self.assertEqual(self.graph.degree_inbound(1), 1)

    def test_degree_outbound(self):
        self.assertEqual(self.graph.degree_outbound(0), 1)
        self.assertEqual(self.graph.degree_outbound(1), 1)

    def test_get_cost(self):
        self.assertEqual(self.graph.get_cost(0, 1), 1)
        self.assertEqual(self.graph.get_cost(1, 2), 2)

    def test_set_cost(self):
        self.graph.set_cost(0, 1, 4)
        self.assertEqual(self.graph.get_cost(0, 1), 4)

    def test_add_vertex(self):
        self.graph.add_vertex(3)
        self.assertEqual(self.graph.is_vertex(3), True)