import java.util.*;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class Graph {
    private Set<Integer> vertices;
    private Map<Edge, Integer> costs;
    private Map<Integer, Set<Integer>> outbound;
    private Map<Integer, Set<Integer>> inbound;

    public Graph(int nr_vertices) {
        vertices = new HashSet<>();
        costs = new HashMap<>();
        outbound = new HashMap<>();
        inbound = new HashMap<>();
        for (int cnt = 0; cnt < nr_vertices; cnt++) {
            addVertex(cnt);
        }
    }

    public Iterator<Integer> vertIter() {
        return vertices.iterator();
    }

    public boolean isVertex(int ver) {
        return vertices.contains(ver);
    }

    public int countVertices() {
        return vertices.size();
    }

    public Iterator<Edge> edgeIter() {
        return costs.keySet().iterator();
    }

    public boolean isAnEdge(int src, int dst) {
        return outbound.containsKey(src) && outbound.get(src).contains(dst);
    }

    public int countEdges() {
        return costs.size();
    }

    public Graph copy() {
        Graph copy = new Graph(0);
        copy.vertices = new HashSet<>(vertices);
        copy.costs = new HashMap<>(costs);
        copy.outbound = new HashMap<>(outbound);
        copy.inbound = new HashMap<>(inbound);
        return copy;
    }

    public Iterator<Integer> sourceIter(int ver) {
        if (!isVertex(ver)) {
            throw new IllegalArgumentException("Vertex is nonexistent.");
        }
        return inbound.get(ver).iterator();
    }

    public Iterator<Integer> destinationIter(int ver) {
        if (!isVertex(ver)) {
            throw new IllegalArgumentException("Vertex is nonexistent.");
        }
        return outbound.get(ver).iterator();
    }

    public int degreeInbound(int ver) {
        if (!inbound.containsKey(ver)) {
            throw new IllegalArgumentException("Vertex is nonexistent.");
        }
        return inbound.get(ver).size();
    }

    public int degreeOutbound(int ver) {
        if (!outbound.containsKey(ver)) {
            throw new IllegalArgumentException("Vertex is nonexistent.");
        }
        return outbound.get(ver).size();
    }

    public int getCost(int src, int dst) {
        Edge edge = new Edge(src, dst);
        if (!costs.containsKey(edge)) {
            throw new IllegalArgumentException("Edge is nonexistent.");
        }
        return costs.get(edge);
    }

    public void setCost(int src, int dst, int cost) {
        Edge edge = new Edge(src, dst);
        if (!costs.containsKey(edge)) {
            throw new IllegalArgumentException("Edge is nonexistent.");
        }
        costs.put(edge, cost);
    }

    public void addVertex(int ver) {
        if (isVertex(ver)) {
            throw new IllegalArgumentException("Vertex already exists.");
        }
        vertices.add(ver);
        outbound.put(ver, new HashSet<>());
        inbound.put(ver, new HashSet<>());
    }

    public void removeVertex(int ver) {
        if (!isVertex(ver)) {
            throw new IllegalArgumentException("Vertex is nonexistent.");
        }
        Set<Integer> tempConnections = new HashSet<>();
        for (int dst : outbound.get(ver)) {
            tempConnections.add(dst);
        }
        for (int dst : tempConnections) {
            removeEdge(ver, dst);
        }
        tempConnections = new HashSet<>();
        for (int src : inbound.get(ver)) {
            tempConnections.add(src);
        }
        for (int src : tempConnections) {
            removeEdge(src, ver);
        }
        outbound.remove(ver);
        inbound.remove(ver);
        vertices.remove(ver);
    }

    public void addEdge(int src, int dst, int cost) {
        if (isAnEdge(src, dst)) {
            throw new IllegalArgumentException("Edge already exists.");
        }
        if (!isVertex(src) || !isVertex(dst)) {
            throw new IllegalArgumentException("Source or destination vertex is nonexistent.");
        }
        Edge edge = new Edge(src, dst);
        costs.put(edge, cost);
        outbound.get(src).add(dst);
        inbound.get(dst).add(src);
    }

    public void removeEdge(int src, int dst) {
        if (!isAnEdge(src, dst)) {
            throw new IllegalArgumentException("Edge is nonexistent.");
        }
        Edge edge = new Edge(src, dst);
        costs.remove(edge);
        outbound.get(src).remove(dst);
        inbound.get(dst).remove(src);
    }

    private class Edge {
        private int src;
        private int dst;

        public Edge(int src, int dst) {
            this.src = src;
            this.dst = dst;
        }

        @Override
        public boolean equals(Object obj) {
            if (this == obj) {
                return true;
            }
            if (obj == null || getClass() != obj.getClass()) {
                return false;
            }
            Edge other = (Edge) obj;
            return src == other.src && dst == other.dst;
        }

        @Override
        public int hashCode() {
            return Objects.hash(src, dst);
        }
    }
}
