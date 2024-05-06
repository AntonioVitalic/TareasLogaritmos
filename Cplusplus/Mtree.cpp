#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

class Point {
public:
    double x; // Coordenada X del punto
    double y; // Coordenada Y del punto

    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}
};

class MTree; // Declaración anticipada para permitir referencia en Entry

class Entry {
public:
    Point p;            // Punto de esta entrada
    double cr;          // Radio de cobertura de todos los puntos del subárbol asociado
    std::shared_ptr<MTree> a; // Dirección del subárbol, usando smart pointers para manejo automático de memoria

    Entry(const Point& point, double radius) : p(point), cr(radius), a(nullptr) {}
};

class MTree {
public:
    std::vector<std::shared_ptr<Entry>> entries; // Uso de vector para manejar dinámicamente las entradas

    MTree() = default;

    void insert(const Point& point, double radius) {
        entries.push_back(std::make_shared<Entry>(point, radius));
    }

    size_t size() const {
        return entries.size();
    }
};

class Query {
public:
    Point p;
    double r;

    Query(const Point& point, double radius) : p(point), r(radius) {}
};

int main() {
    MTree tree;
    tree.insert(Point(0.1, 0.2), 0.05);
    tree.insert(Point(0.3, 0.4), 0.06);

    std::cout << "Tree has " << tree.size() << " entries." << std::endl;

    for (auto& entry : tree.entries) {
        std::cout << "Point (" << entry->p.x << ", " << entry->p.y << ") with radius " << entry->cr << std::endl;
    }

    return 0;
}
