package Dijkstra;

import java.util.ArrayList;
import java.util.List;

import Structs.Graph.Graph;
import Structs.Graph.Node;
import Structs.Q.Q;
import Utils.Pair;

public class Dijkstra {
    
    private static final int ArrayList = 0;

    /*
        TODO: Cambiar el retorno cuando conozca de que son las listas 
        (Pair<List<Distancias>, List<Previos>>)
    */ 
    public <T extends Q> Q algoritmo(Graph grafo, Node raiz) {
        // Paso 1: Crear arreglos de distancias y previos
        Integer n = grafo.size(); // Tamaño del grafo
        List<Double> distancias = new ArrayList<Double>(n);
        List<Node> previos = new ArrayList<Node>(n);

        // Paso 2: Crear Q
        Q q = null;

        // Paso 3: Agregar par (0, raiz) a Q
        q.addPair(new Pair<Double, Node>(0, raiz));

        

        return q;
    }

}
