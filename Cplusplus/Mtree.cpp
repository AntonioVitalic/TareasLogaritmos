#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <memory>

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
        entries.push_back(make_shared<Entry>(point, radius));
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
