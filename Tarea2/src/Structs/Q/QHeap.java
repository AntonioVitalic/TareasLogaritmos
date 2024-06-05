package structs.q;

public class QHeap {
    private int[] heap; // Heap de los nodos (solo tags)
    private int[] pos; // Posicion de los nodos en el heap (el indice de cada posicion es el tag del nodo, ej: pos[3] = 5 significa que el nodo 3 esta en la posicion 5 del heap)
    private double[] dist; // Distancia de cada nodo
    private int size; // Tamaño del heap

    public QHeap(int n) { // Se inicializa con el tamaño de la cantidad de nodos
        heap = new int[n];
        pos = new int[n];
        dist = new double[n];
        size = 0;
    }

    /*
     * Método para agregar un nodo a la cola de prioridad
     */
    public void add(int node, double d) {
        heap[size] = node;
        pos[node] = size;
        dist[size] = d;
        size++;
    }

    /*
     * Método para extraer el nodo con la menor prioridad
     */
    public int extractMin() {
        if (size == 0) {
            return -1;
        }
        int min = heap[0];
        heap[0] = heap[size - 1];
        pos[heap[0]] = 0;
        dist[0] = dist[size - 1];
        size--;
        heapify(0);
        return min;
    }

    /*
     * Método para disminuir la prioridad de un nodo
     */
    public void decreaseKey(int node, double d) {
        int i = pos[node]; // Posicion del nodo en el heap
        dist[i] = d; // Se actualiza la distancia del nodo
        while (i > 0 && dist[parent(i)] > dist[i]) {
            swap(i, parent(i)); // Se intercambia el nodo con su padre (swap reordena tambien las posiciones)
            i = parent(i); // Se actualiza la posicion del nodo
        }
    }

    /*
     * Método para verificar si la cola de prioridad está vacía
     */
    public boolean isEmpty() {
        return size == 0;
    }

    // Funcionalidades internas

    /*
     * Método para obtener el padre de un nodo
     */
    private int parent(int i) {
        return (i - 1) / 2;
    }

    /*
     * Método para obtener los hijos de un nodo
     */
    private int left(int i) {
        return 2 * i + 1;
    }

    /*
     * Método para obtener los hijos de un nodo
     */
    private int right(int i) {
        return 2 * i + 2;
    }

    /*
     * Método para heapify
     */
    private void heapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < size && dist[l] < dist[i]) {
            smallest = l;
        }
        if (r < size && dist[r] < dist[smallest]) {
            smallest = r;
        }
        if (smallest != i) {
            swap(i, smallest);
            heapify(smallest);
        }
    }

    /*
     * Método para intercambiar dos nodos
     */
    private void swap(int i, int j) {
        int temp1 = heap[i];
        heap[i] = heap[j];
        heap[j] = temp1;
        double temp2 = dist[i];
        dist[i] = dist[j];
        dist[j] = temp2;
        pos[heap[i]] = i;
        pos[heap[j]] = j;
    }
}
