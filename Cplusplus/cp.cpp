#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>
#include "MTree.cpp" 

using namespace std;
const int B = (int) ((double) 4096 / sizeof(Entry));
const int b = (int) ceil((double) B / 2);

vector<int> randomIndices(int k, int n) {
    vector<int> indices;
    for (int i = 0; i < k; i++) {
        int ind = rand() % (n-1);
        // Verificar si el índice ya fue seleccionado
        if (find(indices.begin(), indices.end(), ind) != indices.end()) {
            i--;  // Repetir la selección
            continue;
        } 
        indices.push_back(ind);
    }
    return indices;
}

shared_ptr<MTree> AlgoritmoCP(vector<Point>& points) {
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
            double minDist = 2;
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
                    double minDist = 2;
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

    // Paso 6: AlgoritmoCP recursivamente en cada sample
    vector<shared_ptr<MTree>> trees;
    for (int i = 0; i < samples.size(); i++) {
        shared_ptr<MTree> cp = AlgoritmoCP(samples[i]);
        trees.push_back(cp);
    }

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
    vector<shared_ptr<MTree>> treesPrime; // T'
    int h = INT_MAX;
    for (const auto& tree: trees) {
        int local_min = tree->getHeight();
        h = min(h, local_min);
    }

    // Paso 9: por cada Tj, si su altura es igual a h, se añade a T'
    for (int i = 0; i < trees.size(); i++) {
        if (trees[i]->getHeight() == h) { // Si su altura es igual a h se añade a T'
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

    // Paso 10: Definir T_sup como el resultado de la llamada al algoritmo AlgoritmoCP aplicado a F
    shared_ptr<MTree> T_sup = AlgoritmoCP(F);

    // Paso 11: Unir cada Tj en T' a su hoja en T_sup correspondiente al punto pfj en F
    for (int i = 0; i < treesPrime.size(); i++) {
        shared_ptr<MTree> tree = treesPrime[i];
        Point p = F[i];
        T_sup->entries[i]->a = tree;
    }

    // Paso 12: Setear los radios cobertores resultantes para cada entrada en este arbol    
    for (auto& entry : T_sup->entries) {
        double cr = entry->a->maxDistance(entry->p);
        entry->cr = cr;
    }

    // Paso 13: Retornar T_sup
    return T_sup;
}

int main() {
    // vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    // auto result = AlgoritmoCP(points);
    // if (result) {
    //     cout << "Tree has " << result->size() << " entries." << endl;
    // }

    // Crear un conjunto de puntos aleatorios en el rango [0,1] x [0,1]
    srand(time(NULL));

    
    int n = pow(2, 15);
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        points[i] = {static_cast <double> (rand()) / static_cast <double> (RAND_MAX),
                     static_cast <double> (rand()) / static_cast <double> (RAND_MAX)};
    }

    shared_ptr<MTree> result = AlgoritmoCP(points);

    cout<<endl;
    // result->printTree();

    pair<int, vector<Point>> searchResult = result->search(Query(Point(0.5, 0.4), 0.1));

    cout<<"Search result: "<<endl;
    cout<<"Access count: " << searchResult.first << endl;
    for (const auto& point : searchResult.second) {
        cout<<"Point ("<<point.x<<", "<<point.y<<")"<<endl;
    }

    cout<<"Tree root size: "<<result->size()<<endl;
    cout<<"B: "<<B<<endl;
    cout<<"b: "<<b<<endl;
    return 0;
}
