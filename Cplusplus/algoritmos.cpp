#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>

using namespace std;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/*

    Estructuras

*/

class Point;
class MTree;
class Entry;
class Query;
class Cluster;

class Point {
public:
    double x; // Coordenada X del punto
    double y; // Coordenada Y del punto

    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    // Sobrecarga de operadores para comparar puntos
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    double operator*(const Point& p) const {
        // distance
        return (x - p.x)*(x - p.x) + (y - p.y)*(y - p.y);
    }

    double distanceTo(const Point& other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    int closestPoint(vector<Point> points) {
        int index = 0;
        double m = 2;
        for (int i = 0; i < points.size(); i++) {
            if (*this * points[i] < m) {
                index = i;
            }
        }
        return index;
    }
};

class Query {
public:
    Point p;
    double r;

    Query() = default;

    Query(const Point& point, double radius) : p(point), r(radius) {}
};

class Entry {
public:
    Point p;            // Punto de esta entrada
    double cr;          // Radio de cobertura de todos los puntos del subárbol asociado
    shared_ptr<MTree> a; // Dirección del subárbol, usando smart pointers para manejo automático de memoria

    Entry(const Point& point, double radius) : p(point), cr(radius), a(nullptr) {}
    
    Entry(const Point& point, double radius, shared_ptr<MTree> tree) : p(point), cr(radius), a(tree) {}
};

class MTree {
public:
    vector<shared_ptr<Entry>> entries; // Uso de vector para manejar dinámicamente las entradas

    MTree() = default;

    void setEntries(const vector<shared_ptr<Entry>>& e) {
        entries = e;
    }

    void insert(const Point& point, double radius) {
        entries.push_back(make_shared<Entry>(point, radius)); // push_back para agregar una entrada al final del vector
    }

    void insert(const Point& point, double radius, shared_ptr<MTree> tree) {
        entries.push_back(make_shared<Entry>(point, radius, tree));
    }

    size_t size() const {
        return entries.size();
    }

    int getHeight() {
        for (const auto& entry : entries) {
            if (entry->a) {
                return entry->a->getHeight() + 1;
            } else {
                return 1;
            }
        }
    }

    vector<pair<Point,shared_ptr<MTree>>> searchByHeights(int h) {
        vector<pair<Point, shared_ptr<MTree>>> result;
        // Recorrer todas las entradas y subentradas de ser mayor
        for (const auto& entry : entries) {
            if (entry->a) {
                if (entry->a->getHeight() == h) {
                    result.push_back({entry->p, entry->a});
                } else {
                    vector<pair<Point, shared_ptr<MTree>>> subResult = entry->a->searchByHeights(h);
                    result.insert(result.end(), subResult.begin(), subResult.end());
                }
            }
        }
        return result;
    }

    double maxDistance(Point p) {
        // Recorrer hasta las hojas
        double max = 0;
        // calcular la distancia entre el punto y cada entrada con su radio
        for (const auto& entry : entries) {
            double dist = p.distanceTo(entry->p) + entry->cr;
            if (dist > max) {
                max = dist;
            }
        }
        return max;
    }

    void printTree(int depth = 0) const {
        for (int i = 0; i < depth; i++) {
                cout<<"  ";
        }
        cout<<"Depth: "<<depth<<endl;
        for (const auto& entry : entries) {
            for (int i = 0; i < depth; i++) {
                cout<<"  ";
            }
            cout<<"Point ("<<entry->p.x<<", "<<entry->p.y<<") with radius "<<entry->cr<<endl;
            if (entry->a) {
                entry->a->printTree(depth + 1);
            }
        }
    }

    // Funcion de busqueda en el arbol mediante un query
    pair<int, vector<Point>> search(const Query& q) {
        int accessCount = 1; // Initialize access count to 1
        vector<Point> result;
        for (const auto& entry : entries) {
            if (entry->a) { // Internal node
                if (entry->p.distanceTo(q.p) <= entry->cr + q.r) {
                    pair<int, vector<Point>> subResult = entry->a->search(q);
                    accessCount += subResult.first; // Add access count from sub-tree
                    result.insert(result.end(), subResult.second.begin(), subResult.second.end());
                }
            } else { // External node
                if (entry->p.distanceTo(q.p) <= q.r) {
                    result.push_back(entry->p);
                }
            }
        }
        return make_pair(accessCount, result);
    }
};

class Cluster {
public:
    Point medoid;
    vector<Point> points;

    Cluster() = default;

    Cluster(const vector<Point>& ps) : points(ps) {
        calculateMedoid();
    }

    Cluster(const Point& c) : medoid(c) {}

    void insert(const Point& p) {
        points.push_back(p);
    }

    int size() const {
        return points.size();
    }

    Point getMedoid() const {
        return medoid;
    }

    double rcm() {
        double maxRad = 0;
        for (const auto& p : points) {
            double dist = p * medoid;
            if (dist > maxRad) {
                maxRad = dist;
            }
        }
        return maxRad;
    }

    Point calculateMedoid() {
        Point newMedoid;
        double minDist = INT_MAX;
        for (const auto& p : points) {
            double sum = 0.0;
            for (const auto& q : points) {
                sum += p * q; // usamos d cuadrado por eficiencia
            }
            if (sum < minDist) {
                minDist = sum;
                newMedoid = p;
            }
        }
        medoid = newMedoid;
        return medoid;
    }
};

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// Constantes
const int B = (int) ((double) 4096 / sizeof(Entry));
const int b = (int) ceil((double) B / 2);

/*

    Algoritmo CP

*/

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
    for (int i = 0; i < F.size(); i++) {
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


pair<Cluster, Cluster> minMax(Cluster cuc) {
    Cluster c1, c2;
    double minMaxRC = sqrt(2);
    // Se consideran todos los posibles pares de puntos
    for (int i = 0; i < cuc.size(); i++) {
        for (int j = i+1; j < cuc.size(); j++) {
            // Alternadamente se asigna cada punto a c1 o c2
            Cluster c1tmp = c1, c2tmp = c2;
            vector<Point> points = cuc.points;

            c1tmp = Cluster(points[i]);
            c2tmp = Cluster(points[j]);
            points.erase(points.begin() + i);
            points.erase(points.begin() + j);

            // Se inserta el resto de los puntos  por  cercanía, alternadamente
            while (!points.empty()) {
                int idx1 = c1tmp.getMedoid().closestPoint(points);
                c1tmp.insert(points[idx1]);
                points.erase(points.begin() + idx1);
                if (!points.empty()) {
                    int idx2 = c2tmp.getMedoid().closestPoint(points);
                    c2tmp.insert(points[idx2]);
                    points.erase(points.begin() + idx2);
                }
            }

            // Se calcula el radio de c1 y c2
            double r1 = c1tmp.rcm();
            double r2 = c2tmp.rcm();

            // Se actualiza el minMaxRC y los clusters c1 y c2
            if (max(r1, r2) < minMaxRC) {
                minMaxRC = max(r1, r2);
                c1 = c1tmp;
                c2 = c2tmp;
            }
        }
    }
    return {c1, c2};
}

// Funcion cluster
vector<Cluster> cluster(vector<Point>& Cin){
    // Paso 1: Se define Cout = {} y C = {}
    vector<Cluster> Cout, C;

    // Paso 2: Por cada punto en Cin, crear un cluster con ese punto
    for (const auto& p : Cin) {
        Cluster c = Cluster(p);
        c.insert(p);
        C.push_back(c);
    }

    // Paso 3: Mientras C tenga un solo cluster
    while (C.size() > 1) {
        // Paso 3.1: Par de clusters más cercanos
        int idx1=0, idx2=0; // |c1| >= |c2|
        double minDist = 2;        
        for (int i = 0; i < C.size(); i++) {
            for (int j = i+1; j < C.size(); j++) {
                double dist = C[i].getMedoid() * C[j].getMedoid();
                if (dist < minDist) {
                    minDist = dist;
                    if (C[i].size() < C[j].size()) {
                        idx1 = j;
                        idx2 = i;
                    } else {
                        idx1 = i;
                        idx2 = j;
                    }
                }
            }
        }

        if (C[idx1].size() + C[idx2].size() <= B) {
        // Paso 3.2: Si |c1| + |c2| <= B, entonces unir c1 y c2
            Cluster join = C[idx1];
            for (const auto& p : C[idx2].points) {
                join.insert(p);
            }

            join.calculateMedoid();
            if (idx1 > idx2) { // Jerarquia de eliminacion (erase deplaza los elementos a la izquierda)
                C.erase(C.begin() + idx1);
                C.erase(C.begin() + idx2);
            } else {
                C.erase(C.begin() + idx2);
                C.erase(C.begin() + idx1);
            }
            C.push_back(join);
        } else {
        // Paso 3.2: Se remueve c1 de C y se agrega a Cout
            Cout.push_back(C[idx1]);
            C.erase(C.begin() + idx1);
        }
    }
        

    // Paso 4: c ultimo elemento de C
    Cluster c = C[C.size()-1];

    Cluster cPrime;
    if (Cout.size() > 0) {
    // Paso 5: Si |Cout| > 0:
        // Paso 5.1: definir c' como el vecino mas cercano de c en Cout
        double minDist = 2;
        int idx = 0;
        for (int i = 0; i < Cout.size(); i++) {
            double dist = c.getMedoid() * Cout[i].getMedoid();
            if (dist < minDist) {
                minDist = dist;
                idx = i;
            }
        }
        // Remover c' de Cout
        cPrime = Cout[idx];
        Cout.erase(Cout.begin() + idx);
    } // Paso 5.2: Si no c' esta vacio

    // Paso 6: Si |c u c'| <= B:
    for (const auto& p : cPrime.points) { // Union
        c.insert(p);
    }
    c.calculateMedoid();
    if (c.size() <= B) {
        // Paso 6.1: Añadir c u c' a Cout
        Cout.push_back(c);
    } else {
        // Paso 6.2: Aplicar MinMax
        pair<Cluster, Cluster> c1c2 = minMax(c);
        Cout.push_back(c1c2.first);
        Cout.push_back(c1c2.second);
    }

    // Paso 7: Retornar Cout
    return Cout;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

/*

    Algoritmo SS

*/

shared_ptr<Entry> outputHoja(vector<Point>& Cin) {
    // Paso 1: Sea g el medoide de Cin. Sea r=0. Sea C = {} (nodo hoja)
    double r = 0;
    Cluster c = Cluster(Cin);
    Point g = c.calculateMedoid();
    vector<shared_ptr<Entry>> C;

    // Paso 2: por cada p en Cin, añadir (p, null, null) a C y setear r = max(r, d(p, g))
    for (const auto& p : Cin) {
        Entry e = Entry(p, 0);
        shared_ptr<Entry> entry = make_shared<Entry>(e);
        C.push_back(entry);
        double dist = p * g;
        r = max(r, dist);
    }

    // Paso 3: Guardar el puntero C como a
    shared_ptr<MTree> a = make_shared<MTree>();
    a->setEntries(C);
    
    Entry ret = Entry(g, r, a);
    shared_ptr<Entry> retPtr = make_shared<Entry>(ret);
    return retPtr;
}

shared_ptr<Entry> outputInterno(vector<shared_ptr<Entry>>& Cmra) {
    // Paso 1: Sea Cin = {g1, g2, ..., gk} (g_i es el p de cada Entry en Cmra), G el medioide de Cin, R=0, C={} (output interno)
    Cluster Cin;
    for (const auto& entry : Cmra) {
        Cin.insert(entry->p);
    }
    Point G = Cin.calculateMedoid();
    double R = 0;

    // Paso 2: Por cada Entry en Cmra añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
    shared_ptr<MTree> A = make_shared<MTree>();
    double quadDist = 0;
    double cr = 0;
    for (const auto& entry : Cmra) {
        A->insert(entry->p, entry->cr, entry->a);
        if (R < entry->cr + G * entry->p) {
            quadDist = G * entry->p;
            cr = entry->cr;
            R = entry->cr + G * entry->p;
        }
    }
    R = sqrt(quadDist) + cr; // Optimizacion de sqrt

    // Paso 3, 4: Guardar el puntero C como A (check) y retornar (G, R, A)
    Entry ret = Entry(G, R, A);
    shared_ptr<Entry> retPtr = make_shared<Entry>(ret);
    return retPtr;
}

shared_ptr<MTree> AlgoritmoSS(vector<Point>& Cin) {
    // Paso 1: Si |Cin| <= B, se define (g, r, a) = outputHoja(Cin) y se retorna a
    if (Cin.size() <= B) {
        shared_ptr<Entry> entry = outputHoja(Cin);
        return entry->a;
    }

    // Paso 2: Sea Cout = cluster(Cin). Sea C = {}
    vector<Cluster> Cout = cluster(Cin);
    vector<shared_ptr<Entry>> C;

    // Paso 3: Por cada c en Cout, añadir outputHoja(c) a C
    for (const auto& c : Cout) {
        vector<Point> points = c.points;
        shared_ptr<Entry> entry = outputHoja(points);
        C.push_back(entry);
    }

    // Paso 4: Mientras |C| > B
    while (C.size() > B) {
        // Paso 4.1: Sea Cin = {g1, g2, ..., gk} (g_i es el p de cada Entry en C). Sea Cout = cluster(Cin). Sea Cmra = {}
        vector<Point> Cin;
        for (const auto& entry : C) {
            Cin.push_back(entry->p);
        }
        
        vector<Cluster> Cout = cluster(Cin);
        vector<vector<shared_ptr<Entry>>> Cmra;

        // Paso 4.2: por cada c en Cout, sea s = {(g,r,a)_1, (g,r,a)_2, ..., (g,r,a)_m}, se añade s a Cmra
        for (const auto& c : Cout) {
            vector<shared_ptr<Entry>> s;
            for (const auto& entry : C) {
                if (find(c.points.begin(), c.points.end(), entry->p) != c.points.end()) {
                    s.push_back(entry);
                }
            }
            Cmra.push_back(s);
        }

        // Paso 4.3: Sea C = {}
        C.clear();

        // Paso 4.4: Por cada s en Cmra, añadir outputInterno(s) a C
        for (const auto& s : Cmra) {
            vector<shared_ptr<Entry>> vs = s;
            shared_ptr<Entry> entry = outputInterno(vs);
            C.push_back(entry);
        }
    }

    // Paso 5: Sea (g, r, a) = outputInterno(C)
    shared_ptr<Entry> ret = outputInterno(C);

    return ret->a;
}