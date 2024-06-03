package structs.graph;
import java.util.ArrayList;

public class Node {
    public int id; // Identificador del nodo (tag)
    public ArrayList<Integer> adjList; // Lista de adyacencia (como lista por ser dinamica)
    public ArrayList<Double> costList; // Lista de costos, de tipo double debido a que se generan pesos aleatorios y uniformes en el rango (0, 1]

    public Node(int id) {
        this.id = id;
        this.adjList = new ArrayList<>();
        this.costList = new ArrayList<>();
    }

    public void addEdge(int to, double d) {
        adjList.add(to);
        costList.add(d);
    }
}
