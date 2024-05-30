package algoritmo;

import structs.q.QHeap;
import structs.graph.Graph;

public class Dijkstra {
    
    public static void heap(Graph graph, int root) {
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
}
