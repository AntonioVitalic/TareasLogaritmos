#include <iostream>
#include <vector>
#include <algorithm>
#include "MTree.cpp" 

using namespace std;

#define B 4096 / sizeof(Entry)
#define b B / 2

vector<int> randomIndices(int k, int n) {
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
        auto tree = make_shared<MTree>();
        for (const auto& point : points) {
            tree->insert(point, 0.0);
        }
        return tree;
    }

    int n = points.size();

    int k = min(B, n / B);
    vector<Point> F(k); // F
    vector<vector<Point>> samples(k); // F_j
    
    do {
        // Paso 2: Elegir k = min(B, n / B) puntos aleatorios de P, y crear samples
        vector<int> indices = randomIndices(k, n);
        for (int i = 0; i < k; i++) {
            F[i] = points[indices[i]];
        }
        
        // Paso 3: Agrupar segun sample mas cercano
        for (const auto& point : points) {
            double minDist = 1.0;
            int minIndex = 0;
            for (int i = 0; i < k; i++) {
                double dist = distance(point, F[i]);
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = i;
                }
            }
            samples[minIndex].push_back(point);
        }

        // Paso 4: Redistribucion de samples segun |samples| < b
        for (int i = 0; i < k; i++) {
            if (samples[i].size() < b) {
                samples.erase(samples.begin() + i);
                F.erase(F.begin() + i);
                indices.erase(indices.begin() + i);
                i--;  // Ajustar índice del ciclo después de borrar
                k--;  // Reducir número de samples

                // Reasignacion
                for (const auto& point : points) {
                    double minDist = 1.0;
                    int minIndex = 0;
                    for (int i = 0; i < k; i++) {
                        double dist = distance(point, F[i]);
                        if (dist < minDist) {
                            minDist = dist;
                            minIndex = i;
                        }
                    }
                    samples[minIndex].push_back(point);
                }
            }
        }

    } while (k==1); // Paso 5: Si k=1, entonces volver al paso 2

    // Paso 6: CP recursivamente en cada sample
    vector<shared_ptr<MTree>> trees; // T_j
    for (auto& sample : samples) {
        trees.push_back(CP(sample));
    }

    // Paso 7: Si la raiz es tamaño menor que b, se quita esa raiz
    // se elimina el indice y se reduce k, y se agregan sus subarboles
    // a los trees y los puntos a F
    for (int i = 0; i < k; i++) {
        if (trees[i]->size() < b) {
            // Agregar subarboles a trees
            for (auto& entry : trees[i]->entries) {
                shared_ptr<MTree> subtree = entry->a;
                Point p = entry->p;
                trees.push_back(subtree);
                F.push_back(p);
            }
            samples.erase(samples.begin() + i);
            trees.erase(trees.begin() + i);
            i--;  // Ajustar índice del ciclo después de borrar
            k--;  // Reducir número de samples previos (para no evaluar los arboles recien agregados)
        }
    }

    k = F.size();

    // Paso 8: Balanceamiento. Calcular h (altura minima de los trees)
    // Se define T' vacio
    vector<shared_ptr<MTree>> treesPrime(trees.size()); // T'
    int h = INT_MAX;
    for (const auto& tree: trees) {
        int local_min = tree->minHeight();
        h = min(h, local_min);
    }

    // Paso 9: por cada T ...
    for (const auto& tree: trees) {
        if (tree->maxHeight() == h) { // Si su altura es igual a h se añade a T'
            treesPrime.push_back(tree);
        } else { // Si no
            // Se borra el punto en F

        }
    }

//     vector<vector<Point>> clusters;
//     int k;
//     do {
//         k = min(B, static_cast<int>(points.size()) / B);
//         vector<int> indices = randomIndices(k, points.size());

//         clusters.assign(k, vector<Point>());

//         for (const auto& point : points) {
//             double minDist = numeric_limits<double>::max();
//             int minIndex = 0;
//             for (int i = 0; i < k; i++) {
//                 double dist = distance(point, points[indices[i]]);
//                 if (dist < minDist) {
//                     minDist = dist;
//                     minIndex = i;
//                 }
//             }
//             clusters[minIndex].push_back(point);
//         }

//         for (int i = 0; i < k; i++) {
//             if (clusters[i].size() < b) {
//                 clusters.erase(clusters.begin() + i);
//                 indices.erase(indices.begin() + i);
//                 i--;  // Adjust loop index after erase
//                 k--;  // Reduce number of clusters
//             }
//         }
//     } while (k == 1);  // Repeat if there's only one cluster left

//     vector<shared_ptr<MTree>> trees;
//     for (auto& cluster : clusters) {
//         trees.push_back(CP(cluster));
//     }

//     // Assuming further steps to combine these trees as per the algorithm description
//     return nullptr;  // This should be replaced with the actual tree combining logic
// }
}

int main() {
    vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    auto result = CP(points);
    if (result) {
        cout << "Tree has " << result->size() << " entries." << endl;
    }

    cout << sizeof(shared_ptr<Entry>) << endl;
    return 0;
}
