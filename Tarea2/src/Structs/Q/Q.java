package Structs.Q;

import Structs.Graph.Node;
import Utils.Pair;

/**
 * Definimos una estructura Q permita:
    • Almacenar pares de la forma (distancia, nodo).
    • Obtener y eliminar el par con menor distancia.
    • Tener acceso directo desde cada nodo (por medio de un puntero) al par que lo representa,
    además de poder modificar su distancia con la función decreaseKey (en el contexto del
    algoritmo, no existe la necesidad de aumentar la distancia, solo de reducirla).
 */
public interface Q {
    public void addPair(Pair<Double, Node> pair);
    public Pair<Double, Node> getMin();
    public Node getNode(Integer key);
    public void decreaseKey(Node node, Double newDistance);    
}


// La idea es que de la interface Q, se creen las clases QFib extends Q y QHeap extends Q, de modo que
// en estas clases se implementen los métodos (pueden crearse más métodos aquí, de ser necesario)
