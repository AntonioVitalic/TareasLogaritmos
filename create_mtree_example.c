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

// En este archivo, crearemos un M-Tree de ejemplo con un conjunto de puntos P
// y crearemos una funcion auxiliar para imprimir los puntos de un M-Tree

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#include "MTree.c"

#define BLOCK_SIZE 4096
#define RADIUS 0.02
#define B 4096 / sizeof(Entry)

// void pp_mtree(MTree *mtree, int order);
// void pp_entry(Entry *entry, int order);

// // Funcion para imprimir los puntos de un M-Tree
// void printMTree(MTree *mtree) {
//     if (mtree == NULL) {
//         return;
//     }

//     for (int i = 0; i < B; i++) {
//         if (mtree->entries[i].p != NULL) {
//             printf("Punto (%f, %f)\n", mtree->entries[i].p->x, mtree->entries[i].p->y);
//             printf("\t");
//             printMTree(mtree->entries[i].a);
//         }
//     }
// }


// Entry *createEntry(Point *p, double cr, MTree *a) {
//     Entry *entry = malloc(sizeof(Entry));  // Correct type of malloc
//     entry->p = p;
//     entry->cr = cr;
//     entry->a = a;
//     return entry;
// }


// MTree *createMTree(Entry **entries) {
//     MTree *mtree = (MTree *)malloc(sizeof(MTree));
//     mtree->entries = entries;
//     mtree->size = sizeof(*entries) / sizeof(Entry);
//     return mtree;
// }

// Point *createPoint(double x, double y) {
//     Point *point = (Point *)malloc(sizeof(Point));
//     point->x = x;
//     point->y = y;
//     return point;
// }

// void pp_entry(Entry *entry, int order) {
//     // if (!entry) return;
//     // printf("%f", "aaaaaaaaa");
//     // for (int i = 0; i < order; i++) {
//     //     printf("\t");
//     // }
//     // printf("Punto (%f, %f)\n", entry->p->x, entry->p->y);
//     // if (entry->a != NULL) {
//     //     pp_mtree(entry->a, order + 1);
//     // }
// }

// void pp_mtree(MTree *mtree, int order) {
//     if (!mtree) return;
//     for (int i = 0; i < mtree->size; i++) {
//         pp_entry(mtree->entries[i], order);
//     }
// }

int main() {
    printf("%d\n", B);
    printf("%d\n", sizeof(MTree)); 
    printf("%d\n", sizeof(Entry)); 
    printf("%d", sizeof(Point)); 

    return 0;
}
//     srand(time(NULL));

//     Entry **entries = malloc(5 * sizeof(Entry*));
//     for (int i = 0; i < 5; i++) {
//         entries[i] = createEntry(createPoint(i, i-1), 0, NULL);
//     }

//     MTree *leaf = createMTree(entries);

//     Entry **rootEntries = malloc(sizeof(Entry *));
//     rootEntries[0] = createEntry(createPoint(100, 100), 1000, leaf);
//     MTree *root = createMTree(rootEntries);

//     pp_mtree(root, 0);