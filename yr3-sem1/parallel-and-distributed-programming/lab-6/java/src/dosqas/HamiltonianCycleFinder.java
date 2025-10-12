package dosqas;

import java.util.*;
import java.util.concurrent.*;

public class HamiltonianCycleFinder {
    private final List<List<Integer>> graph;
    private final int numThreads;

    public HamiltonianCycleFinder(List<List<Integer>> graph, int numThreads) {
        this.graph = graph;
        this.numThreads = numThreads;
    }

    public List<Integer> findCycle(int start) {
        try (ForkJoinPool pool = new ForkJoinPool(numThreads)) {
            return pool.invoke(new HamiltonianCycleTask(graph, start, new HashSet<>(List.of(start)), new ArrayList<>(List.of(start)), numThreads));
        }
    }

    // RecursiveTask is a specialized subclass of ForkJoinTask, designed for divide and conquer paralellism
    static class HamiltonianCycleTask extends RecursiveTask<List<Integer>> {
        private final List<List<Integer>> graph;
        private final int current;
        private final Set<Integer> visited;
        private final List<Integer> path;
        private final int threadsLeft;

        HamiltonianCycleTask(List<List<Integer>> graph, int current, Set<Integer> visited, List<Integer> path, int threadsLeft) {
            this.graph = graph;
            this.current = current;
            this.visited = visited;
            this.path = path;
            this.threadsLeft = threadsLeft;
        }

        // The compute function represents the core logic of our task
        @Override
        protected List<Integer> compute() {
            // If we visited all the nodes, check if the last node has an edge from it to the starting node (0)
            if (visited.size() == graph.size()) {
                if (graph.get(current).contains(path.getFirst())) {
                    path.add(path.getFirst());
                    return path;
                }
                return null;
            }

            List<Integer> neighbors = new ArrayList<>();
            for (int neighbor : graph.get(current)) {
                // Add only the unvisited neighbors
                if (!visited.contains(neighbor)) {
                    neighbors.add(neighbor);
                }
            }

            if (threadsLeft > 1 && neighbors.size() > 1) {
                // If we got more than 1 thread left to allocate, we split the work between tasks
                List<HamiltonianCycleTask> tasks = new ArrayList<>();
                int[] threadAlloc = allocateThreads(neighbors.size(), threadsLeft);
                for (int i = 0; i < neighbors.size(); i++) {
                    int neighbor = neighbors.get(i);
                    Set<Integer> newVisited = new HashSet<>(visited);
                    newVisited.add(neighbor);
                    List<Integer> newPath = new ArrayList<>(path);
                    newPath.add(neighbor);
                    tasks.add(new HamiltonianCycleTask(graph, neighbor, newVisited, newPath, threadAlloc[i]));
                }
                invokeAll(tasks);
                for (HamiltonianCycleTask task : tasks) {
                    List<Integer> result = task.join();
                    if (result != null) return result;
                }
            } else {
                // Go sequentially otherwise
                for (int neighbor : neighbors) {
                    Set<Integer> newVisited = new HashSet<>(visited);
                    newVisited.add(neighbor);
                    List<Integer> newPath = new ArrayList<>(path);
                    newPath.add(neighbor);
                    HamiltonianCycleTask task = new HamiltonianCycleTask(graph, neighbor, newVisited, newPath, 1);
                    List<Integer> result = task.compute();
                    if (result != null) return result;
                }
            }
            return null;
        }

        // Evenly distribute the remaining threads among available branches
        // Example: 8 threads, 3 branches -> [3, 3, 2]
        private int[] allocateThreads(int branches, int threads) {
            int[] alloc = new int[branches];
            int base = threads / branches;
            int rem = threads % branches;
            for (int i = 0; i < branches; i++) {
                alloc[i] = base + (i < rem ? 1 : 0);
            }
            return alloc;
        }
    }
}