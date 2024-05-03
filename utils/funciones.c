#include "math.h"
#include <stdio.h>
#include "stdlib.h"
#include "../MTree.c"

// Linked list

typedef struct Node {
    Point p;
    struct Node *next;
} Node;

int size(Node *head) {
    int count = 0;
    Node *current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void insert(Node **head, Point p) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->p = p;
    newNode->next = *head;
    *head = newNode;
}

void delete(Node **head, Point p) {
    Node *current = *head;
    Node *prev = NULL;
    while (current != NULL) {
        if (current->p.x == p.x && current->p.y == p.y) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

// Distancia al cuadrado entre 2 puntos
double quadDistance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Distancia entre 2 puntos
double distance(Point p1, Point p2) {
    return sqrt(quad_distance(p1, p2));
}

// Estructura para almacenar los límites del intervalo de confianza
typedef struct {
    double lower;
    double upper;
} ConfidenceInterval;

// Función que calcula el intervalo de confianza para una media, desviación estándar y tamaño de muestra dados
ConfidenceInterval intervaloConfianza(double media, double desviacion, int n) {
    double error = 1.96 * (desviacion / sqrt(n));
    ConfidenceInterval ci;
    ci.lower = media - error;
    ci.upper = media + error;
    return ci;
}