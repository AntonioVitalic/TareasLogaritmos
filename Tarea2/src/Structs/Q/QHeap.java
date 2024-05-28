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
//     public Pair<Double, Node> getMin();
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
    private Map<Node, Integer> nodeToHeapIndex;

    public QHeap(ArrayList<Pair<Double, Node>> elements) {
        this.heap = new ArrayList<>(elements.size());
        this.nodeToHeapIndex = new HashMap<>();
        buildHeap(elements);
    }

    private void buildHeap(ArrayList<Pair<Double, Node>> elements) {
        int n = elements.size();
        for (int i = 0; i < n; i++) {
            heap.add(elements.get(i));
            nodeToHeapIndex.put(elements.get(i).getSecond(), i);
        }
        for (int i = (n / 2) - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }

    @Override
    public void addPair(Pair<Double, Node> pair) {
        heap.add(pair);
        int index = heap.size() - 1;
        nodeToHeapIndex.put(pair.getSecond(), index);
        heapifyUp(index);
    }

    @Override
    public Pair<Double, Node> getMin() {
        if (heap.isEmpty()) return null;
        Pair<Double, Node> min = heap.get(0);
        int lastIndex = heap.size() - 1;
        swap(0, lastIndex);
        heap.remove(lastIndex);
        nodeToHeapIndex.remove(min.getSecond());
        if (!heap.isEmpty()) {
            heapifyDown(0);
        }
        return min;
    }

    @Override
    public void decreaseKey(Node node, Double newDistance) {
        Integer index = nodeToHeapIndex.get(node);
        if (index != null && newDistance < heap.get(index).getFirst()) {
            heap.set(index, new Pair<>(newDistance, node));
            heapifyUp(index);
        }
    }

    private void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap.get(parentIndex).getFirst() > heap.get(index).getFirst()) {
                swap(parentIndex, index);
                index = parentIndex;
            } else {
                break;
            }
        }
    }

    private void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int leftIndex = 2 * index + 1;
            int rightIndex = 2 * index + 2;
            int smallest = index;
            if (leftIndex < size && heap.get(leftIndex).getFirst() < heap.get(smallest).getFirst()) {
                smallest = leftIndex;
            }
            if (rightIndex < size && heap.get(rightIndex).getFirst() < heap.get(smallest).getFirst()) {
                smallest = rightIndex;
            }
            if (smallest != index) {
                swap(index, smallest);
                index = smallest;
            } else {
                break;
            }
        }
    }

    private void swap(int i, int j) {
        Pair<Double, Node> temp = heap.get(i);
        heap.set(i, heap.get(j));
        heap.set(j, temp);
        nodeToHeapIndex.put(heap.get(i).getSecond(), i);
        nodeToHeapIndex.put(heap.get(j).getSecond(), j);
    }
}