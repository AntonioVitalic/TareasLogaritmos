package experiments;

import structs.graph.Graph;
import structs.graph.Node;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Random;
import java.util.Locale;

import algoritmo.Dijkstra;

/*
 * Clase que realiza experimentos para comparar los algoritmos de Dijkstra con heap binario y heap de Fibonacci
 */
public class Experiments {

    public static void main(String[] args) {
        int[] verticesLevels = {10, 12, 14};
        int[] edgesPowers = {16, 17, 18, 19, 20, 21, 22};

        for (int i : verticesLevels) {
            // Crear archivo por cada nivel de vertices
            String filenameHeap = "results_heap_(" + i + ").csv";
            setupCSV(filenameHeap);

            for (int j : edgesPowers) {

                // Crear arreglos para guardar los tiempos de ejecución
                ArrayList<Double> timesHResults = new ArrayList<>();
                ArrayList<Double> timesFResults = new ArrayList<>();

                // 50 iteraciones por cada combinación de vertices y aristas
                int vertices = (int) Math.pow(2, i);
                int edges = (int) Math.pow(2, j);
                
                for (int k = 0; k < 50; k++) {
                    double timeH = runExperiments(vertices, edges, 0);
                    double timeF = runExperiments(vertices, edges, 1);
                    timesHResults.add(timeH);
                    timesFResults.add(timeF);
                }

                // Calcular promedio de tiempos
                double avgTimeH = timesHResults.stream().mapToDouble(Double::doubleValue).average().orElse(0);
                double avgTimeF = timesFResults.stream().mapToDouble(Double::doubleValue).average().orElse(0);

                // Escibir resultados en archivo
                writeResults(filenameHeap, edges, avgTimeH, avgTimeF);
            }
        }  
    }
    
    /*
     * Método para ejecutar los experimentos
     * @param vertices: número de vértices del grafo
     * @param edges: número de aristas del grafo
     * @param algorithm: algoritmo a utilizar (0: heap binario, 1: heap de Fibonacci)
     * @return tiempo de ejecución del algoritmo
     */
    private static double runExperiments(int vertices, int edges, int algorithm) {
        Graph graph = generateGraph(vertices, edges);
        return measureTime(graph, algorithm);
    }

    /*
     * Método para medir el tiempo de ejecución de un algoritmo
     * @param graph: grafo sobre el cual se ejecutará el algoritmo
     * @param algorithm: algoritmo a utilizar (0: heap binario, 1: heap de Fibonacci)
     * @return tiempo de ejecución del algoritmo
     */
    private static double measureTime(Graph graph, int algorithm) {
        long startTime, endTime;
        startTime = System.nanoTime();
        if (algorithm == 0) {
            Dijkstra.heapHeap(graph, 0);
        } else if (algorithm == 1){
            Dijkstra.heapFib(graph, 0);
        }
        endTime = System.nanoTime();
        return (endTime - startTime) / 1e9;
    }

    /*
     * Método para escribir los resultados en un archivo CSV
     * @param filename: nombre del archivo
     * @param edges: número de aristas del grafo
     * @param timeH: tiempo de ejecución del algoritmo con heap binario
     * @param timeF: tiempo de ejecución del algoritmo con heap de Fibonacci
     */
    private static void writeResults(String filename, int edges, double timeH, double timeF) {
        try (PrintWriter out = new PrintWriter(new FileWriter(filename, true))) {
            out.printf(Locale.US, "%d,%.9f,%.9f\n", edges, timeH, timeF);
        } catch (IOException e) {
            System.err.println("Error writing to CSV file: " + e.getMessage());
        }
    }

    /*
     * Método para generar un grafo aleatorio
     * @param vertices: número de vértices del grafo
     * @param edges: número de aristas del grafo
     * @return grafo aleatorio
     */
    private static Graph generateGraph(int vertices, int edges) {
        Graph graph = new Graph(vertices);
        long seed = 123456789;
        Random random = new Random(seed);

        // Añadir nodos y aristas
        for (int i = 0; i < vertices; i++) {
            graph.addNode(new Node(i));
            if (i > 0) {
                int to = random.nextInt(i);
                graph.nodes[i].addEdge(to, 1 - random.nextDouble()); // nextDouble va de [0,1) y queremos (0,1]
                graph.nodes[to].addEdge(i, 1 - random.nextDouble()); // Hacerlo no dirigido
            }
        }

        // Añadir aristas adicionales
        for (int i = 0; i < edges - (vertices - 1); i++) {
            int from = random.nextInt(vertices);
            int to = random.nextInt(vertices);
            double weight = 1 - random.nextDouble(); // nextDouble va de [0,1) y queremos (0,1]
            graph.nodes[from].addEdge(to, weight);
            graph.nodes[to].addEdge(from, weight);
        }

        return graph;
    }

    /*
     * Método para configurar el archivo CSV
     * @param filename: nombre del archivo
     */
    private static void setupCSV(String filename) {
        File file = new File(filename);
        if (!file.exists() || file.length() == 0) {
            try (PrintWriter out = new PrintWriter(new FileWriter(file))) {
                out.println("Edges,TimeHeap,TimeFib");
            } catch (IOException e) {
                System.err.println("Error setting up CSV file: " + e.getMessage());
            }
        }
    }
}