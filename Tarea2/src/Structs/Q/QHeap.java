package Structs.Q;

import java.util.ArrayList;
import java.util.List;

import Structs.Graph.Node;
import Utils.Pair;

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
// Como estructura Q de nombre QHeap, se utiliza un Heap como cola de prioridad, este debe soportar las funcionalidades mencionadas en la descripción del algoritmo, y que se construya en tiempo lineal. En este
// primer algoritmo, para la función decreaseKey se espera un tiempo logarítmico, en el que actualizar
// un elemento de Q reordena la estructura del árbol.

public class QHeap implements Q{
    private List<Pair<Double, Node>> heap;
    private int size;
    private int maxSize;

    public QHeap(int maxSize){
        this.maxSize = maxSize;
        this.size = 0;
        this.heap = new ArrayList<Pair<Double, Node>>();
    }

    public void addPair(Pair<Double, Node> pair){
        if(size == maxSize){
            System.out.println("Heap is full");
            return;
        }
        heap[size] = pair;
        size++;
        heapifyUp(size-1);
    }

    public Pair<Double, Node> getMin(){
        if(size == 0){
            System.out.println("Heap is empty");
            return null;
        }
        Pair<Double, Node> min = heap[0];
        heap[0] = heap[size-1];
        size--;
        heapifyDown(0);
        return min;
    }

    public Node getNode(Integer key){
        
        return heap[key].getSecond();
    }

    public void decreaseKey(Node u, Integer key, Double newDistance){
        /* 
         * 
         for(int i = 0; i < size; i++){
             if(heap[i].getValue().equals(node)){
                 heap[i].setKey(newDistance);
                 heapifyUp(i);
                 return;
                }
            }
        }
        */
        
        
    }

    private void heapifyUp(int i){
        int parent = (i-1)/2;
        if(i > 0 && heap[parent].getKey() > heap[i].getKey()){
            Pair<Double, Node> temp = heap[parent];
            heap[parent] = heap[i];
            heap[i] = temp;
            heapifyUp(parent);
        }
    }

    private void heapifyDown(int i){
        int left = 2*i + 1;
        int right = 2*i + 2;
        int smallest = i;
        if(left < size && heap[left].getKey() < heap[i].getKey()){
            smallest = left;
        }
        if(right < size && heap[right].getKey() < heap[smallest].getKey()){
            smallest = right;
        }
        if(smallest != i){
            Pair<Double, Node> temp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = temp;
            heapifyDown(smallest);
        }
    }

    @Override
    public void decreaseKey() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'decreaseKey'");
    }
    
}
