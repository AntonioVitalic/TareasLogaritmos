// == Método Ciaccia-Patella (CP)
// El algoritmo que se propone en este método realiza un clustering de n puntos P = {$p_1$, ..., $p_n$}, obteniendo un M-tree balanceado T que cumple con tener cada nodo dentro de las
// capacidades permitidas. Se realizan los siguientes pasos:

// Algoritmo CP:

// Input: Un set de puntos P
// 1. Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T.

// 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples $p_f_1$, . . . , $p_f_k$. Se insertan en un conjunto F de samples.

// 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos $F_1$, . . . , $F_k$.

// 4. Etapa de redistribución: Si algún $F_j$ es tal que $|F_j| < b$:

// 4.1 Quitamos $p_f_j$ de F

// 4.2 Por cada $p ∈ F_j$, le buscamos el sample $p_f_l$ más cercano de F y lo añadimos a su conjunto $F_l$.

// 5. Si $|F| = 1$, volver al paso 2.
// 6. Se realiza recursivamente el algoritmo CP en cada $F_j$, obteniendo el árbol $T_j$
// 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina $p_f_j$ de F y se trabaja con sus subárboles como nuevos $T_j$ , . . . , $T_{j+p−1}$, se añaden los puntos pertinentes a F.
// 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles $T_j$. Se define T, inicialmente como un conjunto vacío.

// 9. Por cada $T_j$ , si su altura es igual a h, se añade a 
// $T′$. Si no se cumple:
// 9.1 Se borra el punto pertinente en $F$.

// 9.2 Se hace una búsqueda exhaustiva en $T_j$ de todos los subárboles $T′_1$, . . . , $T′_p$ de altura igual a h. Se insertan estos árboles a $T′$

// 9.3 Se insertan los puntos raíz de $T′_1$, . . . , $T′p$, $p′_f_1$, . . . , $p′_f_p$ en F

// 10. Se define $T_sup$ como el resultado de la llamada al algoritmo CP aplicado a F.

// 11. Se une cada $T_j$ ∈ $T′$ a su hoja en $T_sup$ correspondiente al punto $p_f_j$ ∈ F, obteniendo un nuevo árbol T.

// 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.

// 13. Se retorna T .



#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

typedef struct Point {
    double x; // coordenada x del punto p, tiene doble precisión por enunciado
    double y; // coordenada y del punto p, tiene doble precisión por enunciado
} Point;

typedef struct MTree {
    Point *p; // punto p del nodo
    double cr; // radio de cobertura del nodo
    int n; // cantidad de nodos hijos
    int a; // dirección de acceso
    struct MTree **children; // nodos hijos
} MTree;

// B * sizeof(entry) = 4096 bytes, con entry = estructura Ciaccia-Patella (CP) en este archivo
// luego se despeja B = 4096 / sizeof(entry)
#define B 4096
# define b B/2

// funcion para crear arreglo de numeros aleatorios en rango [0, n]
// Función para crear un array de índices aleatorios no repetidos
int *rndm_indx(int k, int n)
{
    int *indices = (int *)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        indices[i] = rand() % n;
        for (int j = 0; j < i; j++) {
            if (indices[i] == indices[j]) {
                i--; // Redo the current index if duplicate found
                break;
            }
        }
    }
    return indices;
}

// funcion de distancia entre 2 puntos (cuadrada)
double quad_distance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// funcion de distancia entre 2 puntos
double distance(Point p1, Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

MTree* CP ( Point **points ) {
    int n = sizeof(*points)/sizeof(Point);
    if (n <= B) {
        MTree *node = (MTree *)malloc(sizeof(MTree));
        node->p = *points;
        node->cr = 0;
        node->children = NULL;
        return node;
    }

    // escoger k puntos aleatorios
    int k = (B < n) ? B : n;
    Point **points_fk = (Point **)malloc(k * sizeof(Point *));
    
    Point **f_groups = (Point **)malloc(k * sizeof(Point *));


    do {

    int* indx = rndm_indx(k, n);
    for (int i = 0; i < k; i++) {
        points_fk[i] = points[indx[i]];
    }

    // creacion de k grupos de puntos
    for (int i = 0; i < k; i++) {
        f_groups[i] = (Point *)malloc(sizeof(Point));
    }

    // asignacion de puntos a grupos
    for (int i=0; i < n; i++) {
        double min_dist = INFINITY;
        int min_dist_idx = -1;
        for (int j=0; j < k; j++) {
            double dist = quad_distance(*points[i], *points_fk[j]);
            if (dist < min_dist) {
                min_dist = dist;
                min_dist_idx = j;
            }
        }
        *f_groups[min_dist_idx] = *points[i];
        f_groups[min_dist_idx]++;
    }
    
    // redistribución
    for (int i=0; i < k; i++) {
        int len = sizeof(f_groups[i])/sizeof(Point);
        if (len < b) {
            Point *p = points_fk[i];
            if (i < k-1)
                points_fk[i] = points_fk[i+1];
            else
                points_fk[i] = NULL;
            k--;
            for (int j=0; j < k; j++){
                int min_dist = INFINITY;
                int min_dist_idx = -1;
                for (int l=0; l < k; l++) {
                    double dist = quad_distance(f_groups[j][l], *points_fk[l]);
                    if (dist < min_dist) {
                        min_dist = dist;
                        min_dist_idx = l;
                    }
                }
                *f_groups[min_dist_idx] = f_groups[i][j];
            }
        }
    }

    } while (sizeof(f_groups)/sizeof(Point) == 1);

    MTree **children = (MTree **)malloc(k * sizeof(MTree *));
    for (int i=0; i < k; i++) {
        // CP(f_groups[i]);
        MTree* tj = CP(f_groups[i]); // recursión de punto 6

        if (tj->children < b){
            // eliminar points_fk[i] de points
            for (int j=0; j < n; j++) {
                if (points_fk[j] == points_fk[i]) {
                    if (j < n-1)
                        points_fk[j] = points_fk[j+1];
                    else
                        points_fk[j] = NULL;
                    n--;
                    k--;
                }
            }

            // se trabaja con los nodos hijos
            for (int j=0; j < tj->n; j++) {
                children[j] = tj->children[j];
            }

            // añadir los puntos pertinentes a F
            for (int j=0; j < tj->n; j++) {
                points_fk[j] = tj->p;
            }

            

        }

        children[i] = tj;
    }

    return NULL;
};

int main() {
    // printf("B = %f\n", B);
    int* arr = rndm_indx(10, 100);
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}