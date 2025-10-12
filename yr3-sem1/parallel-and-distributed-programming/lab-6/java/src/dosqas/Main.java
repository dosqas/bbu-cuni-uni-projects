package dosqas;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {
    private static List<List<Integer>> createGraph(int size) {
        var result = new ArrayList<List<Integer>>();
        var random = new Random();

        for (int i = 0; i < size; i++) {
            result.add(new ArrayList<>());
            for (int j = 0; j < size; j++) {
                if (random.nextInt(2) == 0) {
                    result.get(i).add(j);
                }
            }
        }

        return result;
    }

    public static void main(String[] args) {
        int size = 10;
        var graph = createGraph(size);

        int numThreads = 10;
        HamiltonianCycleFinder cycleFinder = new HamiltonianCycleFinder(graph, numThreads);
        var path = cycleFinder.findCycle(0);

        if (path != null) {
            System.out.println("Cycle found: " + path);
        }
        else {
            System.out.println("No cycle found");
        }
    }
}