// La búsqueda tiene como input el M-Tree (T ) donde se buscará una query Q = (q, r), donde q es
// un punto y r es el radio de búsqueda. Es decir, (q, r) definen una bola. El objetivo es encontrar todos
// los puntos de T que residen dentro de esta.
// Para realizar la búsqueda, se verifica desde la raíz cada nodo hijo de esta:
// • Si el nodo es una hoja, se verifica para cada entrada si p cumple con dist(p, q) ≤ r. Si es así, se
// agrega p a la respuesta.
// • Si el nodo es interno, se verifica para cada entrada (p, cr, a) si dist(p, q) ≤ cr + r.
// Si es así, se busca en su hijo a posibles respuestas. Si no se cumple esa desigualdad, se descarta.

#include <stdio.h>
#include <stdlib.h>
#include "MTree.c"
#include ".\utils\funciones.c"

void searchMTree(MTree *mtree, Query *query, Point *res, int *res_size) {
    Point q = query->p;
    double r = query->r;
    // Si el nodo es hoja
    if ((*mtree->entries)->cr == 0) {
        //para cada entrada si la distancia entre el punto con el punto de la query es menor o igual  
        for (int i = 0; i < mtree->size; i++) {
            Entry *entry = (mtree->entries)[i];
            Point p = entry->p;
            double dist = distance(*p, *q);
            if (dist <= r) {
                // Reallocar res para añadir un nuevo punto
                *res_size += 1;
                *res = realloc(*res, *res_size * sizeof(Point));
                (*res)[*res_size - 1] = *p;
            }
        }
    }
    // Es nodo interno
    for (int i = 0; i < mtree->size; i++) {
        Entry *entry = (mtree->entries)[i];
        Point p = entry->p;
        double cr = entry->cr;
        double dist = distance(*p, *q);
        if (dist <= r + cr) {
                searchMTree((*mtree->entries)->a, query, res, res_size);
            }
    }
}

int main() {

    return 0;
}