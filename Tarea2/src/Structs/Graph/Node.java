package structs.graph;
import java.util.ArrayList;

/*
 * Clase que implementa un nodo de un grafo
 */
public class Node {
    public int id; // Identificador del nodo (tag)
    public ArrayList<Integer> adjList; // Lista de adyacencia (como lista por ser dinamica)
    public ArrayList<Double> costList; // Lista de costos, de tipo double debido a que se generan pesos aleatorios y uniformes en el rango (0, 1]

    /*
     * Constructor de la clase
     * @param id: identificador del nodo
     */
    public Node(int id) {
        this.id = id;
        this.adjList = new ArrayList<>();
        this.costList = new ArrayList<>();
    }

    /*
     * Método para agregar una arista al nodo
     * @param to: nodo al cual se conecta la arista
     * @param d: costo de la arista
     */
    public void addEdge(int to, double d) {
        adjList.add(to);
        costList.add(d);
    }
}
