package algoritmo;

import structs.q.QFib;
import structs.q.QHeap;
import structs.graph.Graph;

public class Dijkstra {
    
    // Método para ejecutar Dijkstra usando QHeap
    public static void heapHeap(Graph graph, int root) {
        int n = graph.size;
        // Paso 1: Creacion de arreglos de distancia y previos |V|
        double[] dist = new double[n];
        int[] prev = new int[n];

        // Paso 2: Creacion de Q
        QHeap q = new QHeap(n);
        
        // Paso 3: definir dist de raiz como 0 y su previo como -1
        // agregar el par (0, raiz) a Q
        dist[0] = 0;
        prev[0] = -1;
        q.add(0, 0); // Nodo tag 0, distancia 0

        // Paso 4: Inicializar distancias como infinito y previos como indefinidos
        // agregar los pares (infinito, nodo) a Q
        for (int i = 1; i < n; i++) {
            dist[i] = Double.POSITIVE_INFINITY;
            prev[i] = -1;
            q.add(i, Double.POSITIVE_INFINITY);
        }

        // Paso 6: Mientras Q no este vacio
        while (!q.isEmpty()) {
            // Paso 6.a: Extraer min de Q
            int u = q.extractMin();
            // Paso 6.b: Para cada vecino v de u
            for (int i = 0; i < graph.nodes[u].adjList.size(); i++) { // el grafo contiene los nodos enumerados, y cada nodo tiene su adyacencia
                int v = graph.nodes[u].adjList.get(i); // Obtiene cada vecino de u
                double w = graph.nodes[u].costList.get(i); // Obtiene el costo de ir de u a v
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    q.decreaseKey(v, dist[v]);
                }
            }
        }
    }

    // Método para ejecutar Dijkstra usando QFib
    public static void heapFib(Graph graph, int root) {
        int n = graph.size; // Asume que graph.size retorna el número total de nodos
        double[] dist = new double[n];
        int[] prev = new int[n];

        // Crear QFib específicamente
        QFib q = new QFib(n);

        // Inicializar distancias y previos
        for (int i = 0; i < n; i++) {
            dist[i] = Double.POSITIVE_INFINITY;
            prev[i] = -1;
        }
        dist[root] = 0;
        q.add(graph.nodes[root], 0);  // Añadir el nodo raíz con distancia 0

        // Mientras que la cola de prioridad no esté vacía
        while (!q.isEmpty()) {
            int u = q.extractMin();  // u es ahora el índice del nodo con la mínima distancia
            if (dist[u] == Double.POSITIVE_INFINITY) {
                break;  // Todos los nodos restantes son inaccesibles desde la raíz
            }

            // Iterar sobre cada adyacente v de u
            for (int i = 0; i < graph.nodes[u].adjList.size(); i++) {
                int v = graph.nodes[u].adjList.get(i);
                double weight = graph.nodes[u].costList.get(i);

                // Relajar el borde (u, v)
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    q.decreaseKey(graph.nodes[v], dist[v]);  // Asegúrate de que q.decreaseKey espera un Node y un double
                }
            }
        }
    }
}