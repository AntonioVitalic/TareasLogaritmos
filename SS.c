// == Método Sexton-Swinbank (SS)
// El algoritmo que se propone utiliza otro tipo de clustering. Primero se arma distintos grupos de clusters, los cuales son utilizados para armar el M-tree resultante. Primero definiremos los siguientes conceptos:

// a. El medoide de un cluster es un punto g en el cluster tal que no existe otro punto p en el cluster que, al ser nominado como medoide, genere un radio menor al entregado por g
// Sea X = {x1, x2, . . . , xn} un conjunto de n puntos, el medoide de X es el punto g ∈ X tal que g = argmin ∑n i=1 dist(g, xi) donde dist es la función de distancia.

// b. Un cluster puede tener muchos medoides, se define alguna estrategia para definir el medoide primario.

// c. La distancia entre dos clusters es la distancia entre sus medoides primarios.

// d. Dado un cluster c, su vecino más cercano es otro cluster c′ tal que no hay otro cluster que su distancia a c sea menor a la distancia entre c y c′ (se puede tener múltiples vecinos más cercanos).

// e. El par más cercano es un par de clusters c1, c2 tal que dist(c1, c2) ≤ dist(ci, cj ) para cualquier otro par ci, cj .

// Para explicar el algoritmo bulk-loader se definen las siguientes funciones:

// • Cluster: retorna un conjunto de clusters de tamaño entre b y B.

// Input: Un set de puntos Cin de tamaño mínimo b

// 1. Se define C_out = {} y C = {}
// 2. Por cada punto $p ∈ C_in$ se añade {p} a C.
// 3. Mientras |C| > 1:

//     3.1 Sea $c_1$, $c_2$ los pares más cercanos de clusters en C tal que $|c_1| ≥ |c_2|$.
    
//     3.2 Si $|c_1 ∪ c_2| ≤ B$, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
    
//     3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
    
// 4. Sea c el último elemento de C
// 5. Si |Cout| > 0:

//   5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout
  
//   5.2 Si no, se define c′ = {}.

// 6. Si |c ∪ c′| ≤ B:

//   6.1 Añadimos c ∪ c′ a Cout.
  
//   6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.

// 7. Se retorna Cout

// El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros (esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el mínimo radio cobertor máximo.

// • OutputHoja: Retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el radio cobertor y a la dirección del hijo respectivo.

// Input: Cin

// 1. Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).

// 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))

// 3. Guardamos el puntero a C como a

// 4. Retornamos (g, r, a)

// • OutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.

// Input: Cmra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja

// 1. Sea Cin = {g|∃(g, r, a) ∈ Cmra}. G el medoide primario de Cin. Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).

// 2. Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)

// 3. Guardamos el puntero a C como A.

// 4. Retornamos (G, R, A)

// • Algoritmo SS: retorna la raíz del M-tree construído.

// Input: Cin, un conjunto de puntos

// 1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a.

// 2. Sea Cout = Cluster(Cin). Sea C = {}.

// 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C

// 4. Mientras |C| > B:

//   4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}.
  
//   4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
  
//   4.3 Sea C = {}.
  
//   4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C

// 5. Sea (g, r, a) = OutputInterno(C)

// 6. Se retorna a

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "MTree.c"
#include "utils/funciones.c"

#define B 4096 / sizeof(Entry)

/*
El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de
puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros
(esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos
dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el
mínimo radio cobertor máximo.
*/
// void minMax(Point **Cin, Point **c1, Point **c2){
//     double min_dist = INFINITY;
//     for (int i = 0; i < sizeof(Cin)/sizeof(Point); i++){
//         for (int j = i+1; j < sizeof(Cin)/sizeof(Point); j++){
//             double dist = distance(Cin[i], Cin[j]);
//             if (dist < min_dist){
//                 min_dist = dist;
//                 c1 = Cin[i];
//                 c2 = Cin[j];
//             }
//         }
//     }
// }

typedef struct Cluster {
    Point *medoid;
    Point **points;
    int size;
} Cluster;

Point *medioid(Point **p, int n){
    double minSum = INFINITY;
    Point *medoid;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += distance(*p[i], *p[j]);
        }
        if (sum < minSum) {
            minSum = sum;
            medoid = p[i];
        }
    }
    return medoid;
}

Point *ClusterAux(Point *Cin, int n){
    // Paso 1 y 2: Se define Cout = {} y C = {}
    Cluster **cluster = (Cluster**)malloc(sizeof(Cluster*));
    
}

// Point *Cluster(Point **Cin){
//     int n = sizeof(*Cin)/sizeof(Point);

//     // Paso 1 y 2:
//     // 1. Se define Cout = {} y C = {}
//     // 2. Por cada punto $p ∈ C_in$ se añade {p} a C.
//     Node *Cout = NULL, *C = NULL;
//     for (int i = 0; i < n; i++) {
//         insert(&C, (*Cin)[i]);
//     }

//     // Paso 3: 
//     while (size(C)>1){
//         // Paso 3.1: Encontrar los dos clusters más cercanos
//         // Point *c1, *c2;
//         // double min_dist = INFINITY;
//         // for (int i = 0; i < sizeof(C)/sizeof(Point*); i++){
//         //     for (int j = i+1; j < sizeof(C)/sizeof(Point*); j++){
//         //         double dist = distance(C[i], C[j]);
//         //         if (dist < min_dist){
//         //             min_dist = dist;
//         //             c1 = C[i];
//         //             c2 = C[j];
//         //         }
//         //     }
//         // }
//         Node *c1 = NULL, *c2 = NULL;
//         double min_dist = INFINITY;

//         // Encontrar los dos clusters más cercanos en C
//         Node *i = C;
//         while (i != NULL) {
//             Node *j = i->next;
//             while (j != NULL) {
//                 double dist = distance(i->p, j->p);
//                 if (dist < min_dist) {
//                     min_dist = dist;
//                     c1 = i;
//                     c2 = j;
//                 }
//                 j = j->next;
//             }
//             i = i->next;
//         }

//         // Paso 3.2: Unir los dos clusters si su tamaño es menor o igual a B
//     //     if (sizeof(c1) + sizeof(c2) <= B){
//     //         // Remover c1 y c2 de C
//     //         for (int i = 0; i < sizeof(Cout)/sizeof(Point*); i++){
//     //             if (Cout[i] == c1 || Cout[i] == c2){
//     //                 for (int j = i; j < sizeof(Cout)/sizeof(Point*)-1; j++){
//     //                     Cout[j] = Cout[j+1];
//     //                 }
//     //                 break;
//     //             }
//     //         }
//     //         // Añadir c1 ∪ c2 a C
//     //         Point *c = (Point *)malloc(sizeof(Point));
//     //         c = c1;
//     //         for (int i = 0; i < sizeof(c2)/sizeof(Point); i++){
//     //             c[sizeof(c)/sizeof(Point)] = c2[i];
//     //         }
//     //         for (int i = 0; i < sizeof(C)/sizeof(Point*); i++){
//     //             if (C[i] == c1 || C[i] == c2){
//     //                 C[i] = c;
//     //                 break;
//     //             }
//     //         }
//     //     }
//     //     else{ // Paso 3.3: Remover c1 de C y añadirlo a Cout
//     //         // Remover c1 de C
//     //         for (int i = 0; i < sizeof(C)/sizeof(Point*); i++){
//     //             if (C[i] == c1){
//     //                 for (int j = i; j < sizeof(C)/sizeof(Point*)-1; j++){
//     //                     C[j] = C[j+1];
//     //                 }
//     //                 break;
//     //             }
//     //         }
//     //         // Añadir c1 a C
//     //         Point *c = (Point *)malloc(sizeof(Point));
//     //         c = c1;
//     //         for (int i = 0; i < sizeof(C)/sizeof(Point*); i++){
//     //             if (C[i] == c1){
//     //                 C[i] = c;
//     //                 break;
//     //             }
//     //         }
        
//     // }
//     if (c1 != NULL && c2 != NULL) {
//             if (size(c1) + size(c2) <= B) {
//                 // Remover c1 y c2 de C
//                 delete(&C, c1->p);
//                 delete(&C, c2->p);
                
//                 // Añadir c1 ∪ c2 a C
//                 Node *i = c1;
//                 while (i != NULL) {
//                     insert(&c2, i->p);
//                     i = i->next;
//                 }
//             } else {
//                 // Mover c1 a Cout y dejar c2 en C
//                 insert(&Cout, c1->p);
//                 delete(&C, c1->p);
//             }
//         }
//     }

//     // Paso 4: Sea c, el ultimo elemento de C
//     Point *c = C[sizeof(C)/sizeof(Point*)-1];
    
//     // Paso 5: si Cout > 0
//     Point *c_prime;
//     if (sizeof(Cout)/sizeof(Point*) > 0){
//         // 5.1 definimos c′ o c_prime como el vecino más cercano a c en Cout. Removemos c′ de Cout
//         double min_dist = INFINITY;
//         for (int i = 0; i < sizeof(Cout)/sizeof(Point*); i++){
//             double dist = distance(c, Cout[i]);
//             if (dist < min_dist){
//                 min_dist = dist;
//                 c_prime = Cout[i];
//             }
//         }
//         for (int i = 0; i < sizeof(Cout)/sizeof(Point); i++){
//             if (Cout[i] == c_prime){
//                 for (int j = i; j < sizeof(Cout)/sizeof(Point)-1; j++){
//                     Cout[j] = Cout[j+1];
//                 }
//                 break;
//             }
//         }
//         // 5.2 Si no, se define c′ = {}.
//         if (sizeof(c_prime) == 0){
//             c_prime = (Point *)malloc(sizeof(Point));
//         }
//     }

//     // Paso 6: Si |c ∪ c′| ≤ B
//     if (sizeof(c) + sizeof(c_prime) <= B){
//         // 6.1 Añadimos c ∪ c′ a Cout
//         Point *c_union = (Point *)malloc(sizeof(Point));
//         c_union = c;
//         for (int i = 0; i < sizeof(c_prime)/sizeof(Point); i++){
//             c_union[sizeof(c_union)/sizeof(Point)] = c_prime[i];
//         }
//         for (int i = 0; i < sizeof(Cout)/sizeof(Point); i++){
//             if (Cout[i] == c || Cout[i] == c_prime){
//                 Cout[i] = c_union;
//                 break;
//             }
//         }
//     }
         
// }

// OutputHoja: Retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el radio cobertor y a la dirección del hijo respectivo.
// Usamos arreglos

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> // Para DBL_MAX

typedef struct {
    Point g;   // Medoide primario del cluster
    double r;  // Radio cobertor
    Point *a;  // Dirección del hijo respectivo (aquí simplificado como puntero a un arreglo)
} Output;

// Función para calcular el medoide de un conjunto de puntos
Point calculateMedoid(Point *points, int n) {
    double minSum = DBL_MAX;
    Point medoid;
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += distance(points[i], points[j]);
        }
        if (sum < minSum) {
            minSum = sum;
            medoid = points[i];
        }
    }
    return medoid;
}

Output OutputHoja(Point *Cin, int n) {
    Point g = calculateMedoid(Cin, n);
    double r = 0;
    Point *a = malloc(n * sizeof(Point));
    for (int i = 0; i < n; i++) {
        a[i] = Cin[i];  // Simplemente copiamos los puntos a 'a'
        double dist = distance(g, Cin[i]);
        if (dist > r) {
            r = dist;
        }
    }

    Output result = {g, r, a};
    return result;
}

// OutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.

typedef struct {
    Point G;     // Medoide primario del conjunto de medoides
    double R;    // Radio cobertor extendido
    Output *A;   // Dirección del hijo respectivo (representación interna)
} OutputInternal;

OutputInternal OutputInterno(Output *Cmra, int n) {
    Point *medoids = malloc(n * sizeof(Point));
    for (int i = 0; i < n; i++) {
        medoids[i] = Cmra[i].g;
    }
    Point G = calculateMedoid(medoids, n);
    double R = 0;

    Output *A = malloc(n * sizeof(Output));
    for (int i = 0; i < n; i++) {
        A[i] = Cmra[i];
        double extendedRadius = distance(G, Cmra[i].g) + Cmra[i].r;
        if (extendedRadius > R) {
            R = extendedRadius;
        }
    }

    free(medoids);
    OutputInternal result = {G, R, A};
    return result;
}

// Algoritmo SS: retorna la raíz del M-tree construído.
// Nota: se usa el argumento int n, como se hace en la funcion Cluster, para evitar calcular el tamaño de Cin en cada llamada recursiva.

OutputInternal AlgoritmoSS(Point *Cin, int n) {
    if (n <= B) {
        // Si el número de puntos es menor o igual a B, procesamos directamente
        Output hoja = OutputHoja(Cin, n);
        Output *singleNode = malloc(sizeof(Output));
        *singleNode = hoja;
        return OutputInterno(singleNode, 1);
    }

    int coutSize;
    Point **Cout = Cluster(Cin, n, &coutSize);
    Output *C = malloc(coutSize * sizeof(Output));

    for (int i = 0; i < coutSize; i++) {
        C[i] = OutputHoja(Cout[i], 1);
    }

    while (coutSize > B) {
        Point *newCin = malloc(coutSize * sizeof(Point));
        for (int i = 0; i < coutSize; i++) {
            newCin[i] = C[i].g;
        }

        free(Cout);  // Liberar el antiguo Cout
        Cout = Cluster(newCin, coutSize, &coutSize);
        free(C);     // Liberar el antiguo C
        C = malloc(coutSize * sizeof(Output));

        for (int i = 0; i < coutSize; i++) {
            C[i] = OutputHoja(Cout[i], 1);
        }

        free(newCin);  // Liberar el nuevo Cin
    }

    OutputInternal finalNode = OutputInterno(C, coutSize);
    free(C);
    for (int i = 0; i < coutSize; i++) {
        free(Cout[i]);
    }
    free(Cout);
    return finalNode;
}

