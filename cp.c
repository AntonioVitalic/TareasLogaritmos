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
#include "MTree.c"
#include "utils/funciones.c"

// B * sizeof(entry) = 4096 bytes, con entry = estructura Entry
// luego se despeja B = 4096 / sizeof(entry)
#define B 4096 / sizeof(Entry)
#define b B/2

typedef struct {
    Node *head;
    int size;
} Group;

// funcion para crear arreglo de numeros aleatorios en rango [0, n]
// Función para crear un array de índices aleatorios no repetidos
int *rndm_indx(int k, int n) {
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

// Se obtiene un M-Tree balanceado con cada nodo dentro de b y B (excepto raíz)
MTree* CP (Point **points, int n) {
    // Paso 1: Caso base
    if (n <= B) {
        MTree *node = (MTree *) malloc(sizeof(MTree));
        for (int i = 0; i < n; i++) {
            Entry * entry = (Entry *) malloc(sizeof(Entry));
            entry->p = (*points)[i];
            entry->cr = 0;
            entry->a = NULL;
            node->entries[i] = entry;
        }
        return node;
    }

    int k;
    do {
        // Paso 2: Seleccionar k puntos aleatorios
        k = (B < n/B) ? B : n/B;
        int *indices = rndm_indx(k, n);

        // Paso 3: Asignar cada punto a su sample más cercano
        Group *F_k = (Group *)malloc(k * sizeof(Group));
        for (int i = 0; i < k; i++) {
            F_k[i].head = NULL;
            F_k[i].size = 0;
        }
        for (int i = 0; i < n; i++) {
            double min_dist = INFINITY;
            int min_idx = -1;
            for (int j = 0; j < k; j++) {
                double dist = distance((*points)[i], (*points)[indices[j]]);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_idx = j;
                }
            }
            insert(&F_k[min_idx].head, (*points)[i]);
            F_k[min_idx].size++;
        }

        // Paso 4: Redistribución
        for (int i = 0; i < k; i++) {
            if (F_k[i].size < b) {
                // Quitar indice i de indices
                int *new_indices = (int *)malloc((--k) * sizeof(int));
                for (int j = 0; j < k; j++) {
                    if (j < i) {
                        new_indices[j] = indices[j];
                    } else {
                        new_indices[j] = indices[j+1];
                    }
                }
                free(indices);
                indices = new_indices;
                for (Node *node = F_k[i].head; node != NULL; node = node->next) {
                    double min_dist = INFINITY;
                    int min_idx = -1;
                    for (int j = 0; j < k; j++) {
                        double dist = distance(node->p, (*points)[indices[j]]);
                        if (dist < min_dist) {
                            min_dist = dist;
                            min_idx = j;
                        }
                    }
                    insert(&F_k[min_idx].head, node->p);
                    F_k[min_idx].size++;
                }
                Group *new_F_k = (Group *)malloc(k * sizeof(Group));
                for (int j = 0; j < k; j++) {
                    if (j < i) {
                        new_F_k[j] = F_k[j];
                    } else {
                        new_F_k[j] = F_k[j+1];
                    }
                }
                free(F_k);
                F_k = new_F_k;
            } else {
                
            }
        }
    } while (k==1); // Paso 5

    // Paso 6: recursión
    MTree **T_j = (MTree **)malloc(k * sizeof(MTree *));


    return NULL;
}

int main() {
    // printf("B = %f\n", B);
    int* arr = rndm_indx(10, 100);
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}