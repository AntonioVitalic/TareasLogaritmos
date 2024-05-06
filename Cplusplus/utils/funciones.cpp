#include <cmath>
#include <iostream>
#include <stdexcept>
#include "MTree.h"  // Asegúrate de tener este archivo en la ruta correcta o ajusta el include.

// Utilizando namespace std para simplificar el código, aunque se recomienda evitarlo en bibliotecas grandes.
using namespace std;

struct Point {
    double x, y;
};

class Node {
public:
    Point p;
    Node *next;

    Node(Point point) : p(point), next(nullptr) {}
};

class LinkedList {
public:
    Node *head;

    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        clear();
    }

    void insert(Point p) {
        Node *newNode = new Node(p);
        newNode->next = head;
        head = newNode;
    }

    void remove(Point p) {
        Node *current = head, *prev = nullptr;
        while (current != nullptr) {
            if (current->p.x == p.x && current->p.y == p.y) {
                if (prev == nullptr) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
        throw runtime_error("Point not found in list.");
    }

    int size() const {
        int count = 0;
        Node *current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    void clear() {
        while (head != nullptr) {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }
};

double distance(const Point &p1, const Point &p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

struct ConfidenceInterval {
    double lower;
    double upper;

    ConfidenceInterval(double media, double desviacion, int n) {
        double error = 1.96 * (desviacion / sqrt(n));
        lower = media - error;
        upper = media + error;
    }
};

int main() {
    // Ejemplo de uso
    LinkedList list;
    list.insert({1, 2});
    list.insert({3, 4});
    cout << "Size of list: " << list.size() << endl;

    try {
        list.remove({1, 2});
        cout << "Point removed. New size of list: " << list.size() << endl;
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    Point p1 = {0, 0};
    Point p2 = {3, 4};
    cout << "Distance between points: " << distance(p1, p2) << endl;

    ConfidenceInterval ci(100, 15, 30);
    cout << "Confidence Interval: [" << ci.lower << ", " << ci.upper << "]" << endl;

    return 0;
}
