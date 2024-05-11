// == Método Sexton-Swinbank (SS)
// El algoritmo que se propone utiliza otro tipo de clustering. Primero se arma distintos grupos de clusters, los cuales son utilizados para armar el M-tree resultante. Primero definiremos los siguientes conceptos:
//
// a. El medoide de un cluster es un punto g en el cluster tal que no existe otro punto p en el cluster que, al ser nominado como medoide, genere un radio menor al entregado por g
// Sea X = {x1, x2, . . . , xn} un conjunto de n puntos, el medoide de X es el punto g ∈ X tal que g = argmin ∑n i=1 dist(g, xi) donde dist es la función de distancia.
//
// b. Un cluster puede tener muchos medoides, se define alguna estrategia para definir el medoide primario.
//
// c. La distancia entre dos clusters es la distancia entre sus medoides primarios.
//
// d. Dado un cluster c, su vecino más cercano es otro cluster c′ tal que no hay otro cluster que su distancia a c sea menor a la distancia entre c y c′ (se puede tener múltiples vecinos más cercanos).
//
// e. El par más cercano es un par de clusters c1, c2 tal que dist(c1, c2) ≤ dist(ci, cj ) para cualquier otro par ci, cj .
//
// Para explicar el algoritmo bulk-loader se definen las siguientes funciones:
//
// • AlgoritmoCluster: retorna un conjunto de clusters de tamaño entre b y B.
//
// Input: Un set de puntos C_in de tamaño mínimo b
//
// Paso 1. Se define C_out = {} y C = {}
// Paso 2. Por cada punto $p ∈ C_in$ se añade {p} a C.
// Paso 3. Mientras |C| > 1:
//
//  Paso 3.1 Sea $c_1$, $c_2$ los pares más cercanos de clusters en C tal que $|c_1| ≥ |c_2|$.
//  
//  Paso 3.2 Si $|c_1 ∪ c_2| ≤ B$, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
//  
//  Paso 3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
//  
// Paso 4. Sea c el último elemento de C
//
// Paso 5. Si |Cout| > 0:
//
//   Paso 5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout
//
//   Paso 5.2 Si no, se define c′ = {}.
//
// Paso 6. Si |c ∪ c′| ≤ B:
//
//   Paso 6.1 Añadimos c ∪ c′ a Cout.
//
//   Paso 6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
//
// Paso 7. Se retorna Cout
//
// El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de puntos, 
// y alternadamente se van agregando el punto más cercano a alguno de estos centros (esto garantiza que 
// la división sea balanceada) y se calcula el radio cobertor máximo entre estos dos grupos resultantes. 
// Esto se prueba para todo par de puntos y se elige el par que tenga el mínimo radio cobertor máximo.
//
// • AlgoritmoOutputHoja: Retorna una tupla (g, r, a) donde g es el medoide primario de C_in, 
// r es llamado el radio cobertor y a la dirección del hijo respectivo.
//
// Input: C_in
//
// Paso 1. Sea g el medoide primario de C_in. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
//
// Paso 2. Por cada p ∈ C_in: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))
//
// Paso 3. Guardamos el puntero a C como a
//
// Paso 4. Retornamos (g, r, a)
//
// • AlgoritmoOutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de 
// puntos C_in = {g | ∃ (g, r, a) ∈ C_mra}, R el radio cobertor, y A la dirección del hijo respectivo.
//
// Input: C_mra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja
//
// Paso 1. Sea C_in = {g | ∃ (g, r, a) ∈ C_mra}. G el medoide primario de C_in.
// Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).
//
// Paso 2. Por cada (g, r, a) ∈ C_mra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
//
// Paso 3. Guardamos el puntero a C como A.
//
// Paso 4. Retornamos (G, R, A)
//
// • AlgoritmoSS: retorna la raíz del M-tree construído.
//
// Input: C_in, un conjunto de puntos
//
// Paso 1. Si |C_in| ≤ B: Se define (g, r, a) = OutputHoja(C_in) y se retorna a.
//
// Paso 2. Sea Cout = Cluster(C_in). Sea C = {}.
//
// Paso 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
//
// Paso 4. Mientras |C| > B:
//
//   Paso 4.1 Sea C_in = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(C_in). Sea C_mra = {}.
//
//   Paso 4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a C_mra
//
//   Paso 4.3 Sea C = {}.
//
//   Paso 4.4 Por cada s ∈ C_mra: Añadir OutputInterno(s) a C
//
// Paso 5. Sea (g, r, a) = OutputInterno(C)
//
// Paso 6. Se retorna a
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include <tuple>
#include <cstdlib> // Para std::rand y std::srand
#include <ctime>   // Para std::time (para la semilla del generador aleatorio)
#include "MTree.cpp"

const int B = 512 / sizeof(Entry);
const int b = (int) ceil((double) B / 2);
using namespace std;

class Cluster {
public:
    std::vector<Point> points;

    Point calculateMedoid() const {
        Point medoid;
        double minSum = std::numeric_limits<double>::max();

        for (const auto& p1 : points) {
            double sum = 0;
            for (const auto& p2 : points) {
                sum += std::hypot(p1.x - p2.x, p1.y - p2.y);
            }
            if (sum < minSum) {
                minSum = sum;
                medoid = p1;
            }
        }
        return medoid;
    }

    double clusterDistance(const Cluster& other) const {
        // Basic implementation: minimum distance between any two points in the clusters
        double minDistance = std::numeric_limits<double>::max();
        for (const auto& p1 : points) {
            for (const auto& p2 : other.points) {
                double distance = p1.distanceTo(p2);
                if (distance < minDistance) {
                    minDistance = distance;
                }
            }
        }
        return minDistance;
    }
};

// El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de puntos, 
// y alternadamente se van agregando el punto más cercano a alguno de estos centros (esto garantiza que 
// la división sea balanceada) y se calcula el radio cobertor máximo entre estos dos grupos resultantes. 
// Esto se prueba para todo par de puntos y se elige el par que tenga el mínimo radio cobertor máximo.
std::pair<Cluster, Cluster> applyMinMaxSplit(Cluster &cluster1, Cluster &cluster2) {
    // Encontrar el par de puntos (uno de cada cluster) que maximice la distancia entre ellos
    Point maxPoint1 = cluster1.points[0];
    Point maxPoint2 = cluster2.points[0];
    double maxDistance = 0;

    for (const auto& p1 : cluster1.points) {
        for (const auto& p2 : cluster2.points) {
            double dist = p1.distanceTo(p2);
            if (dist > maxDistance) {
                maxDistance = dist;
                maxPoint1 = p1;
                maxPoint2 = p2;
            }
        }
    }

    // Crear dos nuevos clusters usando estos puntos como centros iniciales
    Cluster newCluster1;
    Cluster newCluster2;
    newCluster1.points.push_back(maxPoint1);
    newCluster2.points.push_back(maxPoint2);

    // Distribuir todos los otros puntos al cluster más cercano
    for (const auto& p : cluster1.points) {
        if (p.distanceTo(maxPoint1) < p.distanceTo(maxPoint2)) {
            newCluster1.points.push_back(p);
        } else {
            newCluster2.points.push_back(p);
        }
    }

    for (const auto& p : cluster2.points) {
        if (p.distanceTo(maxPoint1) < p.distanceTo(maxPoint2)) {
            newCluster1.points.push_back(p);
        } else {
            newCluster2.points.push_back(p);
        }
    }

    // Recalcular los medoides después de la redistribución
    newCluster1.calculateMedoid();
    newCluster2.calculateMedoid();

    // Reemplazar los clusters originales con los nuevos clusters
    cluster1 = newCluster1;
    cluster2 = newCluster2;

    return std::make_pair(cluster1, cluster2);
}

// • AlgoritmoCluster: retorna un conjunto de clusters de tamaño entre b y B.
//
// Input: Un set de puntos C_in de tamaño mínimo b
//
// Paso 1. Se define C_out = {} y C = {}
// Paso 2. Por cada punto $p ∈ C_in$ se añade {p} a C.
// Paso 3. Mientras |C| > 1:
//
//  Paso 3.1 Sea $c_1$, $c_2$ los pares más cercanos de clusters en C tal que $|c_1| ≥ |c_2|$.
//  
//  Paso 3.2 Si $|c_1 ∪ c_2| ≤ B$, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
//  
//  Paso 3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
//  
// Paso 4. Sea c el último elemento de C
//
// Paso 5. Si |Cout| > 0:
//
//   Paso 5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout
//
//   Paso 5.2 Si no, se define c′ = {}.
//
// Paso 6. Si |c ∪ c′| ≤ B:
//
//   Paso 6.1 Añadimos c ∪ c′ a Cout.
//
//   Paso 6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
//
// Paso 7. Se retorna Cout
//
// El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de puntos, 
// y alternadamente se van agregando el punto más cercano a alguno de estos centros (esto garantiza que 
// la división sea balanceada) y se calcula el radio cobertor máximo entre estos dos grupos resultantes. 
// Esto se prueba para todo par de puntos y se elige el par que tenga el mínimo radio cobertor máximo.
std::vector<Cluster> AlgoritmoCluster(std::vector<Point> C_in) {
    // Paso 1: Se define C_out = {} y C = {}
    std::vector<Cluster> C_out, C;

    // Paso 2: Por cada punto p ∈ C_in se añade {p} a C
    for (int i = 0; i < C_in.size(); i++){
        Cluster C_temp;
        // agregar C_in[i] a temp
        C_temp.points.push_back(C_in[i]);
        C_temp.calculateMedoid();
        C.push_back(C_temp);
    }

    // Paso 3: Mientras |C| > 1
    while (C.size() > 1) {
        // Paso 3.1: Se define c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|
        double minDistance = std::numeric_limits<double>::max();
        int c1Index = 0, c2Index = 0;

        for (int i = 0; i < C.size(); i++) {
            for (int j = i + 1; j < C.size(); j++) {
                double distance = C[i].clusterDistance(C[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    c1Index = i;
                    c2Index = j;
                }
            }
        }

        // Paso 3.2: Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C
        if (C[c1Index].points.size() + C[c2Index].points.size() <= B) {
            C[c1Index].points.insert(C[c1Index].points.end(), C[c2Index].points.begin(), C[c2Index].points.end());
            C.erase(C.begin() + c2Index);
        } else {
            // Paso 3.3: Si no, se remueve c1 de C y se añade c1 a C_out
            C_out.push_back(C[c1Index]);
            C.erase(C.begin() + c1Index);
        }
    }

    // Paso 4: Sea c el último elemento de C
    Cluster c = C[0];

    // Paso 5: Si |C_out| > 0
    Cluster cPrime = Cluster();
    if (C_out.size() > 0) {
        // Paso 5.1: Se define c′ como el vecino más cercano a c en C_out. Removemos c′ de C_out
        double minDistance = std::numeric_limits<double>::max();
        int cPrimeIndex = 0;

        for (int i = 0; i < C_out.size(); i++) {
            double distance = c.clusterDistance(C_out[i]);
            if (distance < minDistance) {
                minDistance = distance;
                cPrimeIndex = i;
            }
        }

        // Remover c' de C_out
        cPrime = C_out[cPrimeIndex];
        C_out.erase(C_out.begin() + cPrimeIndex);
    } else {
        // Paso 5.2: Si no, se define c′ = {}
        cPrime = Cluster();
    }

    // Paso 6: Si |c ∪ c′| ≤ B
    if (c.points.size() + cPrime.points.size() <= B) {
        // Paso 6.1: Añadimos c ∪ c′ a C_out
        c.points.insert(c.points.end(), cPrime.points.begin(), cPrime.points.end());
        c.calculateMedoid(); // Recalcular el medoide después de la unión
        C_out.push_back(c); // Añadir c ∪ c' a C_out
    } else {
        // Paso 6.2: Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a C_out
        std::pair<Cluster, Cluster> split = applyMinMaxSplit(c, cPrime);
        C_out.push_back(split.first);
        C_out.push_back(split.second);
    }

    // Paso 7: Se retorna C_out
    return C_out;
    
}



// • AlgoritmoOutputHoja: Retorna una tupla (g, r, a) donde g es el medoide primario de C_in,
// r es llamado el radio cobertor y a la dirección del hijo respectivo.
//
// Input: C_in
//
// Paso 1. Sea g el medoide primario de C_in. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
//
// Paso 2. Por cada p ∈ C_in: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))
//
// Paso 3. Guardamos el puntero a C como a
//
// Paso 4. Retornamos (g, r, a)
// Recordar que 
// class Entry {
// public:
//     Point p;            // Punto de esta entrada
//     double cr;          // Radio de cobertura de todos los puntos del subárbol asociado
//     shared_ptr<MTree> a; // Dirección del subárbol, usando smart pointers para manejo automático de memoria

//     Entry(const Point& point, double radius) : p(point), cr(radius), a(nullptr) {}
// };
Entry AlgoritmoOutputHoja(const std::vector<Point>& points) {
    // Paso 1: Sea g el medoide primario de C_in. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
    // Calcular el medoide g (de tipo Point) de C_in
    Cluster C_in;
    C_in.points = points;
    Point g = C_in.calculateMedoid();
    double maxRadius = 0;
    vector<Entry> C; // C = {} (el que corresponderá al nodo hoja).

    // Paso 2: Por cada p ∈ C_in: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))
    for (const auto& point : C_in.points) {
       Entry entry_temp = Entry(point, 0.0);
       C.push_back(entry_temp);
       double dist = g.distanceTo(point);
       if (dist > maxRadius) {
            maxRadius = dist;
       }

    }
    double r = maxRadius; // Paso 2: Seteamos r = max(r, dist(g, p))
    // Crear una instancia de MTree y agregar las entradas
    auto tree = make_shared<MTree>();
    for (auto& entry : C) {
        tree->insert(entry.p, entry.cr);
    }

    // Paso 3 y 4: Configurar y retornar Entry con el árbol
    Entry ret(g, r);
    ret.a = tree;
    return ret;
}

// • AlgoritmoOutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de 
// puntos C_in = {g | ∃ (g, r, a) ∈ C_mra}, R el radio cobertor, y A la dirección del hijo respectivo.
//
// Input: C_mra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja
//
// Paso 1. Sea C_in = {g | ∃ (g, r, a) ∈ C_mra}. G el medoide primario de C_in.
// Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).
//
// Paso 2. Por cada (g, r, a) ∈ C_mra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
//
// Paso 3. Guardamos el puntero a C como A.
//
// Paso 4. Retornamos (G, R, A)
// Recordar que 
// class Entry {
// public:
//     Point p;            // Punto de esta entrada
//     double cr;          // Radio de cobertura de todos los puntos del subárbol asociado
//     shared_ptr<MTree> a; // Dirección del subárbol, usando smart pointers para manejo automático de memoria

//     Entry(const Point& point, double radius) : p(point), cr(radius), a(nullptr) {}
// };

Entry AlgoritmoOutputInterno(const std::vector<Entry>& C_mra) {
    // Paso 1: Sea C_in = {g | ∃ (g, r, a) ∈ C_mra}. G el medoide primario de C_in.
    Cluster C_in;
    for (const auto& entry : C_mra) {
        C_in.points.push_back(entry.p);
    }
    Point G = C_in.calculateMedoid();
    double maxRadius = 0;
    auto C = make_shared<MTree>(); // C = {}, el que corresponderá a un nodo interno.

    // Paso 2: Por cada (g, r, a) ∈ C_mra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
    for (const auto& entry : C_mra) {
        C->insert(entry.p, entry.cr);
        double dist = G.distanceTo(entry.p) + entry.cr; // entry.p es el punto g
        if (dist > maxRadius) {
            maxRadius = dist;
        }
    }
    double R = maxRadius; // Paso 2: Seteamos R = max(R, dist(G, g) + r)
    // Paso 3: Guardamos el puntero a C como A.

    Entry ret = Entry(G, R);
    ret.a = C; // ret.a = A
    
    // Paso 4: Retornamos la tupla (G, R, A)
    return ret;
}

// • AlgoritmoSS: retorna la raíz del M-tree construído.
//
// Input: C_in, un conjunto de puntos
//
// Paso 1. Si |C_in| ≤ B: Se define (g, r, a) = OutputHoja(C_in) y se retorna a.
//
// Paso 2. Sea Cout = Cluster(C_in). Sea C = {}.
//
// Paso 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
//
// Paso 4. Mientras |C| > B:
//
//   Paso 4.1 Sea C_in = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(C_in). Sea C_mra = {}.
//
//   Paso 4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a C_mra
//
//   Paso 4.3 Sea C = {}.
//
//   Paso 4.4 Por cada s ∈ C_mra: Añadir OutputInterno(s) a C
//
// Paso 5. Sea (g, r, a) = OutputInterno(C)
//
// Paso 6. Se retorna a
shared_ptr<MTree> AlgoritmoSS(const std::vector<Point>& C_in) {
    // Paso 1: Verificar si el tamaño de C_in es menor o igual a B.
    if (C_in.size() <= B) {
        // Se define (g, r, a) = OutputHoja(C_in) y se retorna a.
        return AlgoritmoOutputHoja(C_in).a;
    } 

    // Paso 2: Sea C_out = Cluster(C_in). Sea C = {}.
    vector<Cluster> C_out = AlgoritmoCluster(C_in);
    vector<shared_ptr<Entry>> C;

    // Paso 3: Por cada c ∈ C_out: Se añade OutputHoja(c) a C
    for (const auto& c : C_out) {
        C.push_back(make_shared<Entry>(AlgoritmoOutputHoja(c.points)));
    }

    // Paso 4: Mientras |C| > B
    while (C.size() > B) {
        // Paso 4.1: Sea C_in = {g|(g, r, a) ∈ C}. Sea C_out = Cluster(C_in). Sea C_mra = {}.
        vector<Point> C_in_paso_4;
        for (const auto& entry : C) {
            C_in_paso_4.push_back(entry->p);
        }
        vector<Cluster> C_out = AlgoritmoCluster(C_in_paso_4);
        vector<Entry> C_mra;

        // Paso 4.2: Por cada c ∈ C_out: Sea s = {(g, r, a) | (g, r, a) ∈ C ∧ g ∈ c}, se añade s a C_mra
        for (const auto& c : C_out) {
        vector<Entry> s;
            for (const auto& entry : C) {
                if (std::find(c.points.begin(), c.points.end(), entry->p) != c.points.end()) {
                    s.push_back(*entry);
                }
            }
            C_mra.insert(C_mra.end(), s.begin(), s.end());
        }

        // Paso 4.3: Sea C = {}
        C.clear();

        // Paso 4.4: Por cada c ∈ C_out: Por cada s ∈ C_mra: Añadir OutputInterno(s) a C
        for (const auto& entry : C_mra) {
            C.push_back(make_shared<Entry>(AlgoritmoOutputInterno({entry})));
        }
        // C.push_back(make_shared<Entry>(AlgoritmoOutputInterno(C_mra)));
    }

    // Paso 5: Sea (g, r, a) = OutputInterno(C)
    vector<Entry> temp;
    for (const auto& entry : C) {
        temp.push_back(*entry);
    }
    // Paso 6: Se retorna a
    return AlgoritmoOutputInterno(temp).a;
}

// Recordar que
// class MTree {
// public:
//     vector<shared_ptr<Entry>> entries; // Uso de vector para manejar dinámicamente las entradas

//     MTree() = default;

//     void printTree(int depth = 0) const {
//         for (const auto& entry : entries) {
//             cout << string(depth * 2, ' ') << "Punto: (" << entry->p.x << ", " << entry->p.y
//                  << "), Radio: " << entry->cr << "\n";
//             if (entry->a) {
//                 entry->a->printTree(depth + 1);
//             }
//         }
//     }
// }

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const short base = 2;
    const short exponent = 10;

    int n = pow(base, exponent);  // Número de puntos, ajustable según necesidad
    vector<Point> C_in;
    for (int i = 0; i < n; i++) {
        double x = static_cast<double>(rand()) / RAND_MAX;
        double y = static_cast<double>(rand()) / RAND_MAX;
        C_in.emplace_back(x, y);
    }

    // Imprimir la cantidad de puntos generados
    std::cout << "Cantidad de puntos generados: 2^" << log2(C_in.size()) << std::endl;

    // Recordar que hay que usar
    // const int B = 4096 / sizeof(Entry);
    // const int b = ceil(B / 2);
    shared_ptr<MTree> tree = AlgoritmoSS(C_in);

    // Imprimir la estructura del árbol
    cout << "Estructura del arbol MTree:\n";
    if (tree) {
        tree->printTree();  // Si el árbol no es nullptr, imprime su estructura
    } else {
        cout << "El árbol M está vacío.\n";
    }

    return 0;
}
