// == Método Ciaccia-Patella (CP)
// El algoritmo que se propone en este método realiza un clustering de n puntos P = {$p_1$, ..., $p_n$}, obteniendo un M-tree balanceado T que cumple con tener cada nodo dentro de las
// capacidades permitidas. Se realizan los siguientes pasos:
//
// Algoritmo CP:
//
// Input: Un set de puntos P
// 1. Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T.
//
// 2. De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples $p_f_1$, . . . , $p_f_k$. Se insertan en un conjunto F de samples.
//
// 3. Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos $F_1$, . . . , $F_k$.
//
// 4. Etapa de redistribución: Si algún $F_j$ es tal que $|F_j| < b$:
//
// 4.1 Quitamos $p_f_j$ de F
//
// 4.2 Por cada $p ∈ F_j$, le buscamos el sample $p_f_l$ más cercano de F y lo añadimos a su conjunto $F_l$.
//
// 5. Si $|F| = 1$, volver al paso 2.
//
// 6. Se realiza recursivamente el algoritmo CP en cada $F_j$, obteniendo el árbol $T_j$
//
// 7. Si la raíz del árbol es de un tamaño menor a b, se quita esa raíz, se elimina $p_f_j$ de F y se trabaja con sus subárboles como nuevos $T_j$ , . . . , $T_{j+p−1}$, se añaden los puntos pertinentes a F.
//
// 8. Etapa de balanceamiento: Se define h como la altura mínima de los árboles $T_j$. Se define T, inicialmente como un conjunto vacío.
//
// 9. Por cada $T_j$ , si su altura es igual a h, se añade a  $T′$. Si no se cumple:
// 9.1 Se borra el punto pertinente en $F$.
//
// 9.2 Se hace una búsqueda exhaustiva en $T_j$ de todos los subárboles $T′_1$, . . . , $T′_p$ de altura igual a h. Se insertan estos árboles a $T′$
//
// 9.3 Se insertan los puntos raíz de $T′_1$, . . . , $T′p$, $p′_f_1$, . . . , $p′_f_p$ en F
//
// 10. Se define $T_sup$ como el resultado de la llamada al algoritmo CP aplicado a F.
//
// 11. Se une cada $T_j$ ∈ $T′$ a su hoja en $T_sup$ correspondiente al punto $p_f_j$ ∈ F, obteniendo un nuevo árbol T.
//
// 12. Se setean los radios cobertores resultantes para cada entrada en este árbol.
//
// 13. Se retorna T .
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include "MTree.cpp" 

using namespace std;

const int B = 512 / sizeof(Entry);
const int b = ceil(B / 2);

vector<int> randomIndices(int k, int n) { //k indices y rango n
    vector<int> indices;
    for (int i = 0; i < k; i++) {
        int ind = rand() % (n-1);
        // Verificar si el índice ya fue seleccionado
        if (find(indices.begin(), indices.end(), ind) != indices.end()) {
            i--;  // Repetir la selección
            continue;
        } 
        indices.push_back(i);
    }
    return indices;
}

double quadDistance(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

double distance(const Point& p1, const Point& p2) {
    return sqrt(quadDistance(p1, p2));
}

shared_ptr<MTree> CP(vector<Point>& points) {
    // Paso 1: Si |P| <= B, entonces retornar un árbol M-Tree con las entradas de P.
    if (points.size() <= B) {
        shared_ptr<MTree> tree = make_shared<MTree>();
        for (const auto& point : points) {
            tree->insert(point, 0.0);
        }
        return tree;
    }
    // Paso 2: Elegir k = min(B, n / B) puntos aleatorios de P, y crear samples
    int n = points.size();
    int k = min(B, (int)(ceil((double)n / B)));
    cout<<"B: "<<B<<endl;
    cout<<"n: "<<n<<endl;
    cout << "k: " << k << endl;
    vector<Point> F; // F (solo puntos)
    vector<vector<Point>> samples; // F_j
    
    while (true) {
        // Paso 2: Elegir k = min(B, n / B) puntos aleatorios de P, y crear samples
        vector<Point> tmpF; // F (solo puntos)
        vector<vector<Point>> tmpSamples(k); // F_j
        vector<int> indices = randomIndices(k, n);
        for (int i = 0; i < k; i++) {
            tmpF.push_back(points[indices[i]]);
        }
        
        // Paso 3: Agrupar según sample mas cercano
        for (const auto& point : points) {
            double minDist = 1.0;
            int minIndex = 0;
            for (int i = 0; i < tmpF.size(); i++) {
                double dist = point * tmpF[i];
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = i;
                }
            }
            tmpSamples[minIndex].push_back(point);
        }

        // Paso 4: Redistribucion de samples según |samples| < b. Si algún F_j es tal que |F_j| < b:
        for (int i = 0; i < tmpSamples.size();) {
            if (tmpSamples[i].size() < b) {
                // Paso 4.1: Quitar sample p_f_j de F
                tmpF.erase(tmpF.begin() + i);
                // Paso 4.2: Por cada punto p en F_j, buscar el sample más cercano en F
                vector<Point> toAdd = tmpSamples[i];
                tmpSamples.erase(tmpSamples.begin() + i); // Eliminar el sample
                
                for (const auto& point : toAdd) {
                    double minDist = 1.0;
                    int minIndex = 0;
                    for (int j = 0; j < tmpF.size(); j++) {
                        double dist = point * tmpF[j];
                        if (dist < minDist) {
                            minDist = dist;
                            minIndex = j;
                        }
                    }
                    tmpSamples[minIndex].push_back(point);
                }
            } else i++;
        }
        if (tmpSamples.size() == 1) continue; // Paso 5: Si |F|=1, entonces volver al paso 2
        F = tmpF;
        samples = tmpSamples;
        break;
    };

    /*
    vector<shared_ptr<MTree>> trees; // T_j
    for (int i = 0; i < samples.size(); i++) {
        shared_ptr<MTree> cp = CP(samples[i]);
        cout << "Tree " << i << " has " << cp->size() << " entries." << endl;

        // Paso 7: Si la raiz es tamaño menor que b, se quita esa raiz
        if (cp->size() < b) {
            cout << "Removing root with size " << cp->size() << endl;
            // Se elimina el punto en F asociado a este arbol
            F.erase(F.begin() + i);
            // Agregar subarboles a trees
            for (auto& entry : cp->entries) {
                shared_ptr<MTree> subtree = entry->a;
                Point p = entry->p;
                trees.push_back(subtree);
                F.push_back(p);
            }
        } else {
            trees.push_back(cp);
        }
    }
    */
    // Paso 6: CP recursivamente en cada sample
    cout<<"Samples size: "<<samples.size()<<endl;
    for (int i = 0; i < samples.size(); i++) {
        cout<<"Sample "<<i<<" has "<<samples[i].size()<<" entries."<<endl;
    }
    vector<shared_ptr<MTree>> trees;
    for (int i = 0; i < samples.size(); i++) {
        shared_ptr<MTree> cp = CP(samples[i]);
        trees.push_back(cp);
        cout<<"Tree "<<i<<" has "<<cp->size()<<" entries."<<endl;
    }

    cout<<"Trees size: "<<trees.size()<<endl;
    // Paso 7: Si la raiz es tamaño menor que b, se quita esa raiz
    for (int i = 0; i < trees.size();) {
        if (trees[i]->size() < b) {
            // Se elimina el punto en F asociado a este arbol
            F.erase(F.begin() + i);
            // Agregar subarboles a trees
            for (auto& entry : trees[i]->entries) {
                shared_ptr<MTree> subtree = entry->a;
                Point p = entry->p;
                trees.push_back(subtree);
                F.push_back(p);
            }
            trees.erase(trees.begin() + i);
        } else i++;
    }

    // Paso 8: Balanceamiento. Calcular h (altura minima de los trees)
    // Se define T' vacio
    vector<shared_ptr<MTree>> treesPrime(trees.size()); // T'
    int h = INT_MAX;
    for (const auto& tree: trees) {
        int local_min = tree->minHeight();
        h = min(h, local_min);
    }

    // Paso 9: por cada Tj, si su altura es igual a h, se añade a T'
    for (int i = 0; i < trees.size(); i++) {
        if (trees[i]->maxHeight() == h) { // Si su altura es igual a h se añade a T'
            treesPrime.push_back(trees[i]);
        } else { // Si no se cumple:
            // Paso 9.1: Se borra el punto en F
            F.erase(F.begin() + i);

            // Paso 9.2: Busqueda exhaustiva en T de todos los subarboles de altura h. Se insertan estos arboles a T'
            vector<pair<Point, shared_ptr<MTree>>> subTrees = trees[i]->searchByHeights(h);

            // Paso 9.3: Se insertan los puntos raiz de T'1, ..., T'p, p'f1, ..., p'fp en F
            for (auto& subTree : subTrees) {
                F.push_back(subTree.first);
                treesPrime.push_back(subTree.second);
            }
        }
    }

    // Paso 10: Definir T_sup como el resultado de la llamada al algoritmo CP aplicado a F
    shared_ptr<MTree> T_sup = CP(F);

    // Paso 11: Unir cada Tj en T' a su hoja en T_sup correspondiente al punto pfj en F
    for (int i = 0; i < F.size(); i++) {
        shared_ptr<MTree> tree = treesPrime[i];
        Point p = F[i];
        // Buscar hoja en T_sup correspondiente al punto pfj en F
        for (auto& entry : T_sup->entries) {
            if (entry->p == p) {
                entry->a = tree;
                break;
            }
        }
    }

    // Paso 12: Setear los radios cobertores resultantes para cada entrada en este arbol    
    for (auto& entry : T_sup->entries) {
        entry->cr = T_sup->maxDistance(entry->p);
    }

    // Paso 13: Retornar T
    return T_sup;
}

int main() {
    // vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    // auto result = CP(points);
    // if (result) {
    //     cout << "Tree has " << result->size() << " entries." << endl;
    // }

    // Crear un conjunto de puntos aleatorios en el rango [0,1] x [0,1]
    int n = pow(2, 6);
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        points[i] = {static_cast <double> (rand()) / static_cast <double> (RAND_MAX),
                     static_cast <double> (rand()) / static_cast <double> (RAND_MAX)};
    }
    // quitar elementos de un conjunto de puntos
    // points.erase(points.begin() + 1);
    // cout<<points.size()<<endl;

    auto result = CP(points);
    
    // for (auto& entry : result->entries) {
    //     cout << "Point (" << entry->p.x << ", " << entry->p.y << ") with radius " << entry->cr << endl;
    // }

    // cout << sizeof(Entry) << endl;
    return 0;
}
