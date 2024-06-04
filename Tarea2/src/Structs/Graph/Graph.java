package structs.graph;

public class Graph {
    public Node[] nodes; // Lista de nodos enumerados de 0 a n-1
    public int size; // Cantidad de nodos actuales
    public int maxSize; // Cantidad maxima de nodos

    public Graph(int maxSize) { 
        this.maxSize = maxSize; 
        this.size = 0;
        this.nodes = new Node[maxSize];
    }

    public void addNode(Node node) {
        if (size == maxSize) { // Si el grafo esta lleno tira error
            throw new RuntimeException("Graph is full");
        }
        nodes[size] = node;
        size++;
    }
}