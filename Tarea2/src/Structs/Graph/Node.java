package Structs.Graph;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import Utils.Pair;

public class Node {

    // TODO: Refactorizar todos los edges en list
    
    private List<Pair<Double, Node>> edges = new ArrayList<>();
    private Pair<Double, Node> pair;
    
    public void addEdge(Node node, Double weight) {
        edges.put(node, weight);
    }

    public List<Pair<Double, Node>> getEdges() {
        return edges;
    }

    public void delete(){
        for (Pair<Double, Node> edge : edges) {
            edge.getSecond().getEdges().remove(this);
        }
    }

    public void setPair(Pair<Double, Node> pair) {
        this.pair = pair;
    }

    public Pair<Double, Node> getPair() {
        return pair;
    }
}
