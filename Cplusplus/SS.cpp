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
// • Cluster: retorna un conjunto de clusters de tamaño entre b y B.
//
// Input: Un set de puntos Cin de tamaño mínimo b
//
// 1. Se define C_out = {} y C = {}
// 2. Por cada punto $p ∈ C_in$ se añade {p} a C.
// 3. Mientras |C| > 1:
//
//     3.1 Sea $c_1$, $c_2$ los pares más cercanos de clusters en C tal que $|c_1| ≥ |c_2|$.
//  
//     3.2 Si $|c_1 ∪ c_2| ≤ B$, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.
//  
//     3.2 Si no, se remueve c1 de C y se añade c1 a Cout.
//  
// 4. Sea c el último elemento de C
// 5. Si |Cout| > 0:
//
//   5.1 definimos c′ como el vecino más cercano a c en Cout. Removemos c′ de Cout
//
//   5.2 Si no, se define c′ = {}.
//
// 6. Si |c ∪ c′| ≤ B:
//
//   6.1 Añadimos c ∪ c′ a Cout.
//
//   6.2 Si no, dividimos c ∪ c′ en c1 y c2 usando MinMax split policy. Se añaden c1 y c2 a Cout.
//
// 7. Se retorna Cout
//
// El MinMax split policy corresponde a lo siguiente: Se considera todos los posibles pares de puntos, y alternadamente se van agregando el punto más cercano a alguno de estos centros (esto garantiza que la división sea balanceada) y se calcula el radio cobertor máximo entre estos dos grupos resultantes. Esto se prueba para todo par de puntos y se elige el par que tenga el mínimo radio cobertor máximo.
//
// • OutputHoja: Retorna una tupla (g, r, a) donde g es el medoide primario de Cin, r es llamado el radio cobertor y a la dirección del hijo respectivo.
//
// Input: Cin
//
// 1. Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja).
//
// 2. Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p))
//
// 3. Guardamos el puntero a C como a
//
// 4. Retornamos (g, r, a)
//
// • OutputInterno: Retorna (G, R, A) donde G es el medoide primario del conjunto de puntos Cin = {g|∃(g, r, a) ∈ Cmra}, R el radio cobertor, y A la dirección del hijo respectivo.
//
// Input: Cmra, un conjunto de tuplas (g, r, a) retornadas por OutputHoja
//
// 1. Sea Cin = {g|∃(g, r, a) ∈ Cmra}. G el medoide primario de Cin. Sea R = 0. Sea C = {} (el que corresponderá a un nodo interno).
//
// 2. Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r)
//
// 3. Guardamos el puntero a C como A.
//
// 4. Retornamos (G, R, A)
//
// • Algoritmo SS: retorna la raíz del M-tree construído.
//
// Input: Cin, un conjunto de puntos
//
// 1. Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a.
//
// 2. Sea Cout = Cluster(Cin). Sea C = {}.
//
// 3. Por cada c ∈ Cout: Se añade OutputHoja(c) a C
//
// 4. Mientras |C| > B:
//
//   4.1 Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}.
//
//   4.2 Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra
//
//   4.3 Sea C = {}.
//
//   4.4 Por cada s ∈ Cmra: Añadir OutputInterno(s) a C
//
// 5. Sea (g, r, a) = OutputInterno(C)
//
// 6. Se retorna a
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include <cstdlib> // Para std::rand y std::srand
#include <ctime>   // Para std::time (para la semilla del generador aleatorio)
#include "MTree.cpp"   // Asumiendo que MTree.h está configurado correctamente como se describió anteriormente

const int B = 512 / sizeof(Entry);
const int b = ceil(B / 2);

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

std::vector<Cluster> ClusterPoints(const std::vector<Point>& points, int b, int B) {
    std::vector<Cluster> clusters;

    // Inicialmente, cada punto es un cluster
    for (const auto& point : points) {
        clusters.push_back({std::vector<Point>{point}});
    }

    bool merged = true;
    while (merged) {
        merged = false;
        double minDistance = std::numeric_limits<double>::max();
        std::pair<int, int> closestPair;

        // Find the closest pair of clusters that can be merged without exceeding B
        for (int i = 0; i < clusters.size(); i++) {
            for (int j = i + 1; j < clusters.size(); j++) {
                double distance = clusters[i].clusterDistance(clusters[j]);
                int newSize = clusters[i].points.size() + clusters[j].points.size();
                if (distance < minDistance && newSize <= B) {
                    minDistance = distance;
                    closestPair = {i, j};
                    merged = true;
                }
            }
        }

        if (merged) {
            // Merge the closest pair of clusters
            auto& cluster1 = clusters[closestPair.first];
            auto& cluster2 = clusters[closestPair.second];
            cluster1.points.insert(cluster1.points.end(), cluster2.points.begin(), cluster2.points.end());
            cluster1.calculateMedoid();  // Update the medoid
            clusters.erase(clusters.begin() + closestPair.second);  // Remove the second cluster
        }
    }

    // Remove clusters smaller than b
    clusters.erase(std::remove_if(clusters.begin(), clusters.end(),
                                  [b](const Cluster& cluster) { return cluster.points.size() < b; }),
                   clusters.end());

    return clusters;
}

struct Output {
    Point g;
    double r;
    std::shared_ptr<MTree> a;

    Output(const Point& medoid, double radius, std::shared_ptr<MTree> subtree)
        : g(medoid), r(radius), a(std::move(subtree)) {}
};

Output OutputHoja(const Cluster& cluster) {
    auto a = std::make_shared<MTree>();
    Point g = cluster.calculateMedoid();
    double r = 0;

    for (const auto& p : cluster.points) {
        a->insert(p, r);  // Asumiendo que MTree tiene una función de inserción para Point
        double dist = std::hypot(g.x - p.x, g.y - p.y);
        if (dist > r) r = dist;
    }

    return {g, r, a};
}

struct OutputInternal {
    Point G;
    double R;
    std::shared_ptr<MTree> A;

    OutputInternal(const Point& medoid, double radius, std::shared_ptr<MTree> subtree)
        : G(medoid), R(radius), A(std::move(subtree)) {}
};

OutputInternal OutputInterno(const std::vector<Output>& outputs) {
    std::vector<Point> medoids;
    std::transform(outputs.begin(), outputs.end(), std::back_inserter(medoids),
                   [](const Output& output) { return output.g; });

    Cluster medoidCluster{medoids};
    Point G = medoidCluster.calculateMedoid();
    double R = 0;
    auto A = std::make_shared<MTree>();

    for (const auto& output : outputs) {
        double extendedRadius = std::hypot(G.x - output.g.x, G.y - output.g.y) + output.r;
        if (extendedRadius > R) R = extendedRadius;
        A->insert(output.g, output.r);
    }

    return {G, R, A};
}

OutputInternal AlgoritmoSS(const std::vector<Point>& points, int b, int B) {
    if (points.size() <= B) {
        Cluster cluster{points};
        return OutputInterno({OutputHoja(cluster)});
    }

    auto clusters = ClusterPoints(points, b, B);
    std::vector<Output> outputs;
    std::transform(clusters.begin(), clusters.end(), std::back_inserter(outputs),
                   [](const Cluster& cluster) { return OutputHoja(cluster); });

    return OutputInterno(outputs);
}

int main() {
    // Inicialización del generador de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Configuración de los parámetros del algoritmo
    int n = pow(2, 10);  // Número de puntos
    // int b = 3;    // Tamaño mínimo de cada cluster
    // int B = 100;   // Tamaño máximo de cada cluster

    // Crear un conjunto de puntos aleatorios
    std::vector<Point> points;
    for (int i = 0; i < n; i++) {
        double x = static_cast<double>(rand()) / RAND_MAX;  // Número aleatorio entre 0 y 1
        double y = static_cast<double>(rand()) / RAND_MAX;  // Número aleatorio entre 0 y 1
        points.push_back(Point(x, y));
    }

    // Aplicar el algoritmo Sexton-Swinbank
    auto output = AlgoritmoSS(points, b, B);

    // Imprimir la información del nodo raíz del árbol M resultante
    std::cout << "Raiz del M-Tree resultante:\n";
    std::cout << "Punto medoide: (" << output.G.x << ", " << output.G.y << ")\n";
    std::cout << "Radio cobertor: " << output.R << "\n";
    std::cout << "Numero de entradas en el arbol: " << output.A->size() << std::endl;

    // Opcional: Visualizar más detalles del árbol resultante
    for (const auto& entry : output.A->entries) {
        std::cout << "Punto: (" << entry->p.x << ", " << entry->p.y << ") "
                  << "Radio: " << entry->cr << std::endl;
    }

    return 0;
}