#include "math.h"

// Distancia al cuadrado entre 2 puntos
double quadDistance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// Distancia entre 2 puntos
double distance(Point p1, Point p2) {
    return sqrt(quad_distance(p1, p2));
}
//intervalo de confianza 95%
pair<double, double> intervaloConfianza(double media, double desviacion, int n) {
    double error = 1.96 * (desviacion / sqrt(n));
    return (media - error, media + error);
}