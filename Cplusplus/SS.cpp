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
#include <algorithm>
#include "MTree.h"  // Assuming MTree.h is properly set up as described previously

class Cluster {
public:
    Point medoid;
    std::vector<Point> points;
};

double distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

Point calculateMedoid(const std::vector<Point>& points) {
    double minSum = std::numeric_limits<double>::max();
    Point medoid;
    for (const auto& p1 : points) {
        double sum = 0;
        for (const auto& p2 : points) {
            sum += distance(p1, p2);
        }
        if (sum < minSum) {
            minSum = sum;
            medoid = p1;
        }
    }
    return medoid;
}

std::vector<Cluster> ClusterPoints(const std::vector<Point>& points, int b, int B) {
    // Simplified placeholder for actual clustering logic
    std::vector<Cluster> clusters;
    // Logic to split points into clusters goes here
    return clusters;
}

struct Output {
    Point g;
    double r;
    std::shared_ptr<MTree> a;
};

Output OutputHoja(const std::vector<Point>& points) {
    Point g = calculateMedoid(points);
    double r = 0;
    auto a = std::make_shared<MTree>();

    for (const auto& p : points) {
        a->insert(p);  // Assuming MTree has an insert function for Point
        double dist = distance(g, p);
        if (dist > r) r = dist;
    }

    return {g, r, a};
}

struct OutputInternal {
    Point G;
    double R;
    std::shared_ptr<MTree> A;
};

OutputInternal OutputInterno(const std::vector<Output>& outputs) {
    std::vector<Point> medoids;
    for (const auto& output : outputs) {
        medoids.push_back(output.g);
    }

    Point G = calculateMedoid(medoids);
    double R = 0;
    auto A = std::make_shared<MTree>();

    for (const auto& output : outputs) {
        double extendedRadius = distance(G, output.g) + output.r;
        if (extendedRadius > R) R = extendedRadius;
        A->insert(output.g, output.r);  // Assuming MTree can insert with radius
    }

    return {G, R, A};
}

OutputInternal AlgoritmoSS(const std::vector<Point>& points, int b, int B) {
    if (points.size() <= B) {
        return OutputInterno({OutputHoja(points)});
    }

    auto clusters = ClusterPoints(points, b, B);
    std::vector<Output> outputs;
    for (const auto& cluster : clusters) {
        outputs.push_back(OutputHoja(cluster.points));
    }

    return OutputInterno(outputs);
}

int main() {
    std::vector<Point> points = {{0.0, 0.0}, {1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}, {4.0, 4.0}};
    int b = 2, B = 5;  // Example thresholds

    auto result = AlgoritmoSS(points, b, B);
    std::cout << "Resulting MTree has a root medoid at (" << result.G.x << ", " << result.G.y << ") with radius " << result.R << std::endl;

    return 0;
}
