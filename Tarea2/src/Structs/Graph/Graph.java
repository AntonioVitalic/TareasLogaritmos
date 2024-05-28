package Structs.Graph;

import java.util.HashSet;
import java.util.Set;

public class Graph {
    
    private Set<Node> nodes = new HashSet<Node>();

    public Integer size() {
        return nodes.size();
    }

}
