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
    public void decreaseKey();
}