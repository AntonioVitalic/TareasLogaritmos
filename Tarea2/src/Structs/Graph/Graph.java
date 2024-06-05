package structs.graph;

/*
 * Clase que implementa un grafo
 */
public class Graph {
    public Node[] nodes; // Lista de nodos enumerados de 0 a n-1
    public int size; // Cantidad de nodos actuales
    public int maxSize; // Cantidad maxima de nodos

    /*
     * Constructor de la clase
     * @param maxSize: cantidad maxima de nodos que puede tener el grafo
     */
    public Graph(int maxSize) { 
        this.maxSize = maxSize; 
        this.size = 0;
        this.nodes = new Node[maxSize];
    }

    /*
     * Método para agregar un nodo al grafo
     */
    public void addNode(Node node) {
        if (size == maxSize) { // Si el grafo esta lleno tira error
            throw new RuntimeException("Graph is full");
        }
        nodes[size] = node;
        size++;
    }
}