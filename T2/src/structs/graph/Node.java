package structs.graph;
import java.util.ArrayList;

public class Node {
    public int id; // Identificador del nodo (tag)
    public ArrayList<Integer> adjList; // Lista de adyacencia (como lista por ser dinamica)
    public ArrayList<Integer> costList; // Lista de costos

    public Node(int id) {
        this.id = id;
        this.adjList = new ArrayList<>();
        this.costList = new ArrayList<>();
    }

    public void addEdge(int to, int cost) {
        adjList.add(to);
        costList.add(cost);
    }
}
