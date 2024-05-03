/*
Un M-Tree es un árbol que está compuesto de nodos que contiene entradas (p, cr, a), donde p es
un punto, cr es el radio cobertor (covering radius) de este subárbol (la máxima distancia que hay
entre p y cualquier punto del subárbol relacionado a su entrada) y a una dirección en disco a la
página de su hijo identificado por la entrada de su nodo interno. Si el nodo corresponde a una hoja,
por simplicidad asumiremos cr y a nulos. Para contexto de esta tarea utilizaremos las coordenadas
de p y cr como reales de doble precisión (double).
De acuerdo a esto, diremos que cada nodo tendrá como capacidad B entradas en disco, es decir, que
el tamaño de un nodo es a lo más B · sizeof(entry).

Por simplicidad en la realización de la tarea, haremos todos los procesos en memoria principal
y simularemos los accesos a disco (acceder a un nodo del árbol equivale a una lectura de bloque
en disco). Se pide comparar la cantidad de accesos (simulados) a bloques de disco de la búsqueda.
Definiremos el tamaño de un bloque de disco como 4096 Bytes.
Tanto para el set de puntos P, como para los puntos del set de consultas Q, para las coordenadas
se debe utilizar valores aleatorios reales de doble precisión (double) uniformemente distribuidos en
el rango [0, 1]. El radio cobertor también corresponde a un valor real de doble precisión. El tamaño
del atributo a de las entradas corresponde al tamaño de un puntero en memoria. Con todo esto,
más el tamaño del bloque de disco mencionado, se debe estimar los valores de b y B. El radio r de
las consultas debe ser de 0.02 (lo que retorna aproximadamente un 0.12 % de los puntos del conjunto).
*/

typedef struct Point Point;
typedef struct Entry Entry;
typedef struct MTree MTree;
typedef struct Query Query;

struct Point{
    double x; // Coordenada X del punto
    double y; // Coordenada Y del punto
};

struct Entry{
    Point p; // Punto de esta entrada
    double cr; //Radio de cobertura de todos los puntos del subárbol asociado.
    MTree *a; //Dirección en disco de subárbol (tamaño corresponde al tamaño de un puntero en memoria)
};

struct MTree {
    Entry **entries;
    int size;  // cantidad de entradas, la idea es que
    // size = sizeof(entries) / sizeof(Entry);
};

struct Query{
    Point *p;
    double r;
};