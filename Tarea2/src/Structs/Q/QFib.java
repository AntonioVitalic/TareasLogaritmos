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


// La idea es que de la interface Q, se creen las clases QFib extends Q y QHeap extends Q, de modo que
// en estas clases se implementen los métodos (pueden crearse más métodos aquí, de ser necesario)

// Como estructura Q, se utiliza una cola de Fibonacci como cola de prioridad, que permita las
// mismas operaciones, pero implemente decreaseKey en un tiempo constante. En el libro “Introduction
// to Algorithms” de Cormen et al. (ver la bibliografía del curso) hay una descripción detallada de la
// implementación de las colas de Fibonacci.


public class QFib implements Q{

    @Override
    public void addPair(Pair<Integer, Node> pair) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'addPair'");
    }

    @Override
    public void getMin() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getMin'");
    }

    @Override
    public Node getNode(Integer key) {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getNode'");
    }

    @Override
    public void decreaseKey() {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'decreaseKey'");
    }
    
}
