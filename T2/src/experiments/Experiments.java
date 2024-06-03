package experiments;

import structs.graph.Graph;
import structs.graph.Node;
import structs.q.QHeap;
import structs.q.QFib;

import java.util.Random;

import algoritmo.Dijkstra;

public class Experiments {

    public static void main(String[] args) {
        int[] verticesLevels = {10, 12, 14};
        int[] edgesPowers = {16, 18, 20, 22};

        for (int verticesExp : verticesLevels) {
            for (int edgesExp : edgesPowers) {
                runExperiments((int) Math.pow(2, verticesExp), (int) Math.pow(2, edgesExp));
            }
        }
    }

    private static void runExperiments(int vertices, int edges) {
        Graph graph = generateGraph(vertices, edges);
        measurePerformance(graph);
    }

    private static Graph generateGraph(int vertices, int edges) {
        Graph graph = new Graph(vertices);
        Random random = new Random();

        // Añadir nodos y aristas
        for (int i = 0; i < vertices; i++) {
            graph.addNode(new Node(i));
            if (i > 0) {
                int to = random.nextInt(i);
                graph.nodes[i].addEdge(to, random.nextDouble());
                graph.nodes[to].addEdge(i, random.nextDouble()); // Hacerlo no dirigido
            }
        }

        // Añadir aristas adicionales
        for (int i = 0; i < edges - (vertices - 1); i++) {
            int from = random.nextInt(vertices);
            int to = random.nextInt(vertices);
            double weight = random.nextDouble();
            graph.nodes[from].addEdge(to, weight);
            graph.nodes[to].addEdge(from, weight);
        }

        return graph;
    }

    private static void measurePerformance(Graph graph) {
        
        long startTime, endTime;

        // Medición para QHeap
        startTime = System.nanoTime();
        Dijkstra.heapHeap(graph, 0);
        endTime = System.nanoTime();
        System.out.println("Tiempo con QHeap: " + (endTime - startTime) / 1e9 + " segundos");

        // Medición para QFib
        startTime = System.nanoTime();
        Dijkstra.heapFib(graph, 0);
        endTime = System.nanoTime();
        System.out.println("Tiempo con QFib: " + (endTime - startTime) / 1e9 + " segundos");
    }
}
