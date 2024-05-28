package Structs.Graph;

import java.util.HashMap;
import java.util.Map;

public class Node {
    
    Map <Node, Double> edges = new HashMap<Node, Double>();
    
    public void addEdge(Node node, Double weight) {
        edges.put(node, weight);
    }

    public void delete(){
        for (Node node : edges.keySet()) {
            node.edges.remove(this);
        }
        edges.clear();
    }
}
