package Dijkstra;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import Structs.Graph.Graph;
import Structs.Graph.Node;
import Structs.Q.Q;
import Structs.Q.QHeap;
import Structs.Q.QFib;
import Utils.Pair;

// Algoritmo de Dijkstra
// Dado un nodo raíz para un grafo con pesos positivos en sus aristas, encuentra el árbol de caminos
// más cortos para ese nodo raíz, de modo que para cualquier otro nodo en el grafo, la distancia entre
// la raíz y ese nodo es mínima para las aristas pertenecientes al árbol que los conectan. Este algoritmo
// se describe de la siguiente manera:
// 1. Definimos dos arreglos de tamaño |V |, distancias y previos.
// 2. Definimos una estructura Q (ojo que nosotros implementamos primero un interface Q, y luego class QFib extends Q, y 
//    class QHeap extends Q) que permita:
//     • Almacenar pares de la forma (distancia, nodo).
//     • Obtener y eliminar el par con menor distancia.
//     • Tener acceso directo desde cada nodo (por medio de un puntero) al par que lo representa,
//     además de poder modificar su distancia con la función decreaseKey (en el contexto del
//     algoritmo, no existe la necesidad de aumentar la distancia, solo de reducirla).
// 3. Definimos la distancia del nodo raíz como 0, su nodo previo como −1, y agregamos el par
// (distancia = 0, nodo = raíz) a Q.
// 4. Por cada nodo v distinto de la raíz en el grafo:
//     • Definimos distancias[v] como infinita y previos[v] como indefinido.
//     • Agregamos el par (distancia = ∞, nodo = v) a Q.
// 5. Se espera que la creación de Q se resuelva por medio de un Heapify, que transforme un array
// con las distancias en una cola en tiempo lineal (O(n)).
// 6. Mientras Q no se encuentre vacío, repetimos:
//     a) Obtenemos el par (d, v) con menor distancia en Q y lo eliminamos.
//     b) Por cada vecino u del nodo v:
//         i. Si la distancia guardada para u (distancias[u]) es mayor a la distancia guardada para
//         v (distancias[v]) más el peso de la arista (u, v), actualizamos el valor de la distancia
//         de u, guardamos v como el nodo previo de u y actualizamos la distancia del par que
//         representa al nodo u en Q utilizando decreaseKey.
// 7. Retornamos el arreglo de previos y distancias

public class Dijkstra {
    
    private static final int ArrayList = 0;

    /*
        TODO: Cambiar el retorno cuando conozca de que son las listas 
        (Pair<List<Distancias>, List<Previos>>)
    */ 
    public static Q heap(Graph grafo, Node raiz) {
        // Paso 1: Crear arreglos de distancias y previos
        int n = grafo.size(); // Tamaño del grafo
        List<Double> distancias = new ArrayList<Double>(n);
        List<Node> previos = new ArrayList<Node>(n);

        // Paso 2: Crear Q
        Q q = new QHeap(n);

        // Paso 3: Agregar par (0, raiz) a Q
        q.addPair(new Pair<Double, Node>(0.0, raiz));

        // Paso 4: Por cada nodo != raiz, agregar par (infinito, nodo) a Q
        for (Node node : grafo.getNodes()) {
            if (node != raiz) {
                distancias.add(Double.POSITIVE_INFINITY);
                previos.add(null);
                q.addPair(new Pair<Double, Node>(Double.POSITIVE_INFINITY, node));
            }
        }
        

        // Paso 5: Heapify (arriba)

        // Paso 6: Mientras que Q no esté vacío
        while (!q.isEmpty()) {
            // Obtener el par (d, v) de menor distancia
            Pair<Double, Node> par = q.extractMin();

            Double dv = distancias.get(q.indexOf(par.getSecond()));

            // Por cada vecino u de v:
            for (Map.Entry<Node, Double> entry : par.getSecond().getEdges().entrySet()) {
                Node u = entry.getKey();
                Double weight = entry.getValue();
                
                // Si la distancia de u es mayor a la distancia de v + peso de la arista (u, v)
                // Actualizar la distancia de u, guardar v como previo de u y actualizar la distancia de u en Q
                Double du = distancias.get(q.indexOf(u));
                if (du > dv + weight) {
                    distancias.set(q.indexOf(u), dv + weight);
                    previos.set(q.indexOf(u), par.getSecond());
                    q.decreaseKey(u, dv + weight);
                }
            }
        }

        return q;
    }

}
