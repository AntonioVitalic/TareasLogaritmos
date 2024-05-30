package Experiments;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import Structs.Graph.Graph;
import Structs.Graph.Node;
import Structs.Q.Q;
import Dijkstra.Dijkstra;

public class Experiments {
    
    public static void main(String[] args) {
        int[] verticesLevels = {10, 12, 14};  // i ∈ {10, 12, 14}
        int[] edgesPowers = {16, 18, 20, 22}; // j ∈ {16, 18, 20, 22}

        for (int vExp : verticesLevels) {
            int vertices = (int) Math.pow(2, vExp);
            for (int eExp : edgesPowers) {
                int edges = (int) Math.pow(2, eExp);
                runExperiment(vertices, edges);
            }
        }
    }

    private static void runExperiment(int vertices, int edges) {
        List<Double> resultsHeap = new ArrayList<>();
        List<Double> resultsFib = new ArrayList<>();

        for (int trial = 0; trial < 50; trial++) {
            Graph graph = generateGraph(vertices, edges);
            Node startNode = graph.getNodes().iterator().next();

            // Dijkstra with Heap
            long start = System.nanoTime();
            Q qHeap = Dijkstra.heap(graph, startNode);
            long end = System.nanoTime();
            resultsHeap.add((end - start) / 1e9); // convert to seconds

            // // Dijkstra with Fibonacci Heap
            // Q qFib = new QFib(vertices);
            // start = System.nanoTime();
            // Dijkstra.heap(graph, startNode, qFib);
            // end = System.nanoTime();
            // resultsFib.add((end - start) / 1e9); // convert to seconds
        }

        // Output the results
        System.out.println("Heap results: " + resultsHeap);
        System.out.println("Fibonacci Heap results: " + resultsFib);
        // TODO: Add further analysis, possibly exporting data or generating charts
    }

    private static Graph generateGraph(int vertices, int edges) {
        Graph graph = new Graph();
        Random rand = new Random();

        Node[] allNodes = new Node[vertices];
        for (int i = 0; i < vertices; i++) {
            allNodes[i] = new Node();
            graph.addNode(allNodes[i]);
            if (i > 0) {
                int connectTo = rand.nextInt(i);
                double weight = rand.nextDouble();
                allNodes[i].addEdge(allNodes[connectTo], weight);
                allNodes[connectTo].addEdge(allNodes[i], weight);  // For undirected graph
            }
        }

        // Add remaining edges
        int extraEdges = edges - (vertices - 1);
        for (int j = 0; j < extraEdges; j++) {
            int from = rand.nextInt(vertices);
            int to = rand.nextInt(vertices);
            double weight = rand.nextDouble();
            allNodes[from].addEdge(allNodes[to], weight);
            allNodes[to].addEdge(allNodes[from], weight);  // For undirected graph
        }

        return graph;
    }
}