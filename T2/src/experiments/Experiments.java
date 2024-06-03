package experiments;

import structs.graph.Graph;
import structs.graph.Node;

import java.io.File;
import java.io.FileWriter; // Por si queremos crear archivos CSV como en la tarea 1, para graficar cada par (i, j)
import java.io.IOException;
import java.io.PrintWriter;


import java.util.Random;

import algoritmo.Dijkstra;

public class Experiments {

    public static void main(String[] args) {
        int[] verticesLevels = {10, 12, 14};
        int[] edgesPowers = {16, 17, 18, 19, 20, 21, 22};
    
        for (int i : verticesLevels) {
            for (int j : edgesPowers) {
                runExperiments((int) Math.pow(2, i), (int) Math.pow(2, j), i, j);
            }
        }
    }
    
    
    

    private static void runExperiments(int vertices, int edges, int i, int j) {
        Graph graph = generateGraph(vertices, edges);
        String filenameHeap = "results_heap_(" + i + "," + j + ").csv";
        String filenameFib = "results_fib_(" + i + "," + j + ").csv";
        setupCSV(filenameHeap);
        setupCSV(filenameFib);
        measurePerformance(graph, filenameHeap, filenameFib, vertices, edges);
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

    private static void measurePerformance(Graph graph, String filenameHeap, String filenameFib, int vertices, int edges) {
        try (
            PrintWriter outHeap = new PrintWriter(new FileWriter(filenameHeap, true));
            PrintWriter outFib = new PrintWriter(new FileWriter(filenameFib, true))
        ) {
            long startTime, endTime;
    
            // Medición para QHeap
            startTime = System.nanoTime();
            Dijkstra.heapHeap(graph, 0);
            endTime = System.nanoTime();
            double timeHeap = (endTime - startTime) / 1e9;
            outHeap.printf("%d,%d,%.9f\n", vertices, edges, timeHeap);
            System.out.printf("QHeap - V: %d, E: %d, Time: %.9f s\n", vertices, edges, timeHeap);
    
            // Medición para QFib
            startTime = System.nanoTime();
            Dijkstra.heapFib(graph, 0);
            endTime = System.nanoTime();
            double timeFib = (endTime - startTime) / 1e9;
            outFib.printf("%d,%d,%.9f\n", vertices, edges, timeFib);
            System.out.printf("QFib - V: %d, E: %d, Time: %.9f s\n", vertices, edges, timeFib);
    
        } catch (IOException e) {
            System.err.println("Error writing to CSV file: " + e.getMessage());
        }
    }
    

    private static void setupCSV(String filename) {
    File file = new File(filename);
    if (!file.exists() || file.length() == 0) {
        try (PrintWriter out = new PrintWriter(new FileWriter(file))) {
            out.println("Vertices,Edges,Time");
        } catch (IOException e) {
            System.err.println("Error setting up CSV file: " + e.getMessage());
        }
    }
}

    
    
    
    
}
