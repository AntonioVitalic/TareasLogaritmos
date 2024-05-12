#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <time.h>
#include "MTree.cpp"

using namespace std;
const int B = (int) ((double) 4096 / sizeof(Entry));
const int b = (int) ceil((double) B / 2);

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


int main() {

    srand(static_cast<unsigned int>(time(nullptr)));

    // Crear un conjunto de puntos aleatorios en el rango [0,1] x [0,1]
    int n = pow(2, 12);
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        points[i] = {static_cast <double> (rand()) / static_cast <double> (RAND_MAX),
                     static_cast <double> (rand()) / static_cast <double> (RAND_MAX)};
    }

    shared_ptr<MTree> result = AlgoritmoSS(points);

    cout<<endl;
    // result->printTree();

    pair<int, vector<Point>> searchResult = result->search(Query(Point(0.5, 0.4), 0.1));

    cout<<"Search result: "<<endl;
    cout<<"Access count: " << searchResult.first << endl;
    for (const auto& point : searchResult.second) {
        cout<<"Point ("<<point.x<<", "<<point.y<<")"<<endl;
    }

    cout<<"Tree root size: "<<result->size()<<endl;
    return 0;
}
