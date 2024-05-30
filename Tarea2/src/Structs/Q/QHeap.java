package Structs.Q;

import Structs.Graph.Node;
import Utils.Pair;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Definimos una estructura Q permita:
    • Almacenar pares de la forma (distancia, nodo).
    • Obtener y eliminar el par con menor distancia.
    • Tener acceso directo desde cada nodo (por medio de un puntero) al par que lo representa,
    además de poder modificar su distancia con la función decreaseKey (en el contexto del
    algoritmo, no existe la necesidad de aumentar la distancia, solo de reducirla).
 */
// public interface Q {
//     public void addPair(Pair<Double, Node> pair);
//     public Pair<Double, Node> extractMin();
//     public Node getNode(Double key);
//     public void decreaseKey();
// }

// Algoritmo 1: Dijkstra con Heap
// Como estructura Q de nombre QHeap, se utiliza un Heap como cola de prioridad, este debe soportar las funcionalidades
// mencionadas en la descripción del algoritmo, y que se construya en tiempo lineal. En este
// primer algoritmo, para la función decreaseKey se espera un tiempo logarítmico, en el que actualizar
// un elemento de Q reordena la estructura del árbol.

public class QHeap implements Q {
    private ArrayList<Pair<Double, Node>> heap;

    public QHeap(int size) {
        this.heap = new ArrayList<>(size);
    }

    public boolean isEmpty() {
        return heap.isEmpty();
    }

    public int indexOf(Node node) {
        return heap.indexOf(node.getPair());
    }

    @Override
    public void addPair(Pair<Double, Node> pair) {
        pair.getSecond().setPair(pair);
        heap.add(pair);
    }

    @Override
    public Pair<Double, Node> extractMin() {
        if (heap.isEmpty()) return null;
        // Extraer el minimo asumiendo que el heap es min-heap
        // y actualizar los indices

        Pair<Double, Node> min = heap.get(0);
        heap.set(0, heap.get(heap.size() - 1));
        heap.remove(heap.size() - 1);
        heapify();

        return min;
    }

    private void heapify() {
        int index = 0;
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < heap.size() && heap.get(left).getFirst() < heap.get(smallest).getFirst()) {
                smallest = left;
            }

            if (right < heap.size() && heap.get(right).getFirst() < heap.get(smallest).getFirst()) {
                smallest = right;
            }

            if (smallest != index) {
                swap(index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }

    @Override
    public void decreaseKey(Node node, Double newDistance) {
        int index = heap.indexOf(node.getPair());
        heap.get(index).setFirst(newDistance);

        while (index > 0 && heap.get(getParent(index)).getFirst() > heap.get(index).getFirst()) {
            swap(index, getParent(index));
            index = getParent(index);
        }
    }

    private int getParent(int i) {
        System.out.println("a");
        return (i - 1) / 2;
    }

    private void swap(int i, int j) {
        Pair<Double, Node> temp = heap.get(i);
        heap.set(i, heap.get(j));
        heap.set(j, temp);
    }
}