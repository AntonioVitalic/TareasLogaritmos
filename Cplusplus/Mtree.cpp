#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>
#include <climits>


using namespace std;

class Point;
class MTree;
class Entry;
class Query;

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
        return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
};

class Entry {
public:
    Point p;            // Punto de esta entrada
    double cr;          // Radio de cobertura de todos los puntos del subárbol asociado
    shared_ptr<MTree> a; // Dirección del subárbol, usando smart pointers para manejo automático de memoria

    Entry(const Point& point, double radius) : p(point), cr(radius), a(nullptr) {}
};

class MTree {
public:
    vector<shared_ptr<Entry>> entries; // Uso de vector para manejar dinámicamente las entradas

    MTree() = default;

    void insert(const Point& point, double radius) {
        entries.push_back(make_shared<Entry>(point, radius)); // push_back para agregar una entrada al final del vector
    }

    size_t size() const {
        return entries.size();
    }

    int maxHeight() {
        if (entries.empty()) {
            return 0;
        } else {
            int max = 0;
            for (const auto& entry : entries) {
                if (entry->a) {
                    int height = entry->a->maxHeight();
                    if (height > max) {
                        max = height;
                    }
                }
            }
            return max + 1;
        }
    }

    int minHeight() {
        if (entries.empty()) {
            return 0;
        } else {
            int min = INT_MAX;
            for (const auto& entry : entries) {
                if (entry->a) {
                    int height = entry->a->minHeight();
                    if (height < min) {
                        min = height;
                    }
                }
            }
            return min + 1;
        }
    }

    vector<pair<Point,shared_ptr<MTree>>> searchByHeights(int h) {
        vector<pair<Point, shared_ptr<MTree>>> result;
        // Recorrer todas las entradas y subentradas de ser mayor
        for (const auto& entry : entries) {
            if (entry->a) {
                if (entry->a->maxHeight() == h) {
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
            double dist = sqrt(p * entry->p) + entry->cr;
            if (dist > max) {
                max = dist;
            }
        }
    }
};

class Query {
public:
    Point p;
    double r;

    Query(const Point& point, double radius) : p(point), r(radius) {}
};

// int main() {
//     MTree tree;
//     tree.insert(Point(0.1, 0.2), 0.05);
//     tree.insert(Point(0.3, 0.4), 0.06);

//     cout << "Tree has " << tree.size() << " entries." << endl;

//     for (auto& entry : tree.entries) {
//         cout << "Point (" << entry->p.x << ", " << entry->p.y << ") with radius " << entry->cr << endl;
//     }

//     cout << sizeof(Entry) << endl;

//     return 0;
// }
