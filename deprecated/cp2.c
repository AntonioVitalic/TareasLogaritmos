#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define B 4096 / sizeof(Entry)
# define b B/2

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct MTree {
    Point p;
    double cr;
    int n;
    int a;
    struct MTree **children;
} MTree;

int *rndIndx(int k, int n)
{
    int *indices = (int *)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++) {
        indices[i] = rand() % n;
        for (int j = 0; j < i; j++) {
            if (indices[i] == indices[j]) {
                i--;
                break;
            }
        }
    }
    return indices;
}

// Distancia al cuadrado entre 2 puntos
double quadDistance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Distancia entre 2 puntos
double distance(Point p1, Point p2) {
    return sqrt(quad_distance(p1, p2));
}

// Creacion de un nodo
MTree* createNode(Point *p, double cr, int n, int a) {
    MTree *node = (MTree *)malloc(sizeof(MTree));
    node->p = *p;
    node->cr = cr;
    node->n = n;
    node->a = a;
    node->children = NULL;
    return node;
}

void insertNearestSample(Point *p, Point **plist, Point ***pgroups, int k){
    double min = INFINITY;
    int idx = -1;
    for (int i = 0; i < k; i++){
        double dist = distance(*p, (*plist)[i]);
        if (dist < min){
            min = dist;
            idx = i;
        }
    }
    (*pgroups)[idx] = (Point*) malloc(sizeof(Point));
    *(*pgroups)[idx] = *p;
    (*pgroups)[idx]++;
}

void redistribute(Point ***pgroups, int *k){
    // Recuperar el grupo k
    Point *group = (*pgroups)[k];

    // Eliminar de la lista de grupos
    (*pgroups)[*k] = NULL;
    (*k)--;
    

    // Asignar a los grupos mas cercanos

}

MTree* CP(Point **points){
    int n = sizeof(*points)/sizeof(Point);

    // Paso 1: Caso tamaño menor o igual al bloque
    if (n <= B){
        return createNode(*points, 0, n, 0);
    }

    // Paso 2: Escoger k puntos aleatorios (samples)
    int k = (B < n) ? B : n;
    int *indices = rndIndx(k, n);
    Point *samples = (Point *)malloc(k * sizeof(Point));
    for (int i = 0; i < k; i++) {
        samples[i] = *points[indices[i]];
    }

    // Paso 3: Asignacion de puntos a los samples mas cercanos
    Point **f_k = (Point *)malloc(k * sizeof(Point*));
    for (int i=0; i < n; i++) {
        insertNearestSample(points[i], samples, &f_k, k);
    }

    // Paso 4: Redistribucion por cada grupo con cantidad < b
    for (int i = 0; i < k; i++){
        if (f_k[i] < b){
            
        }
    }
}

int main() {
    printf("%d", sizeof(MTree));
    return 0;
}