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