#include <iostream>
#include <algorithm>
#include <memory>
#include <limits.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <time.h>
#include "algoritmos.cpp"

using namespace std;

// Promedio
double mean(const vector<double>& v) {
    double sum = 0;
    for (double x : v) {
        sum += x;
    }
    return sum / v.size();
}

// Desviación estándar
double standardDeviation(const vector<double>& v) {
    double m = mean(v);
    double sum = 0;
    for (double x : v) {
        sum += (x - m) * (x - m);
    }
    return sqrt(sum / v.size());
}

// Error estándar
double standardError(const vector<double>& v) {
    return standardDeviation(v) / sqrt(v.size());
}

// Intervalo de confianza
pair<double, double> confidenceInterval(const vector<double>& v) {
    double m = mean(v);
    double s = standardDeviation(v);
    double t = 1.96; // 95% de confianza
    double e = t * s / sqrt(v.size());
    return {m - e, m + e};
}

// Para  generar los 2^n puntos (Generar P)
vector<Point> generatePoint(int n) {
    vector<Point> points;
    for (int i = 0; i < n; i++) {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;
        points.push_back(Point(x, y));
    }
    return points;
}

// Para generar 100 querys (Generar Q)
vector<Query> generateQuerys() {
    vector<Query> querys;
    for (int i = 0; i < 100; i++) {
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;
        double r = (double) rand() / RAND_MAX;
        querys.push_back(Query(Point(x, y), r));
    }
    return querys;
}

// Para testear el algoritmo CP
void testCP(vector<Point>& points, const vector<Query>& querys, ofstream& file) {
    auto start = chrono::high_resolution_clock::now();
    shared_ptr<MTree> tree = AlgoritmoCP(points);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> methodDuration = end - start;

    vector<double> accesses;
    vector<double> times;
    for (const Query& query : querys) {
        auto start = chrono::high_resolution_clock::now();
        pair<int, vector<Point>> result = tree->search(query);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        times.push_back(duration.count());
        accesses.push_back((double)result.first);
    }

    double searchTime = 0;
    for (double time : times) {
        searchTime += time;
    }
    double meanTime = mean(times);
    pair<double, double> interval = confidenceInterval(accesses);

    file<< methodDuration.count() << "," << searchTime << "," << meanTime << "," << interval.first << "," << interval.second << endl;
}

// Para testear el algoritmo SS
void testSS(vector<Point>& points, const vector<Query>& querys, ofstream& file) {
    auto start = chrono::high_resolution_clock::now();
    shared_ptr<MTree> tree = AlgoritmoSS(points);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> methodDuration = end - start;

    vector<double> accesses;
    vector<double> times;
    for (const Query& query : querys) {
        auto start = chrono::high_resolution_clock::now();
        pair<int, vector<Point>> result = tree->search(query);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
        times.push_back(duration.count());
        accesses.push_back((double)result.first);
    }

    double searchTime = 0;
    for (double time : times) {
        searchTime += time;
    }
    double meanTime = mean(times);
    pair<double, double> interval = confidenceInterval(accesses);

    file<< methodDuration.count() << "," << searchTime << "," << meanTime << "," << interval.first << "," << interval.second << endl;
}

int main() {
    srand(time(NULL));

    // Potencias de 2 de inicio a fin (solo exponenetes)
    int inicio = 6;
    int fin = 7;


    // Para cada potencia de 2
    for (int i = inicio; i <= fin; i++) {
        int n = pow(2, i);

        ofstream fileCP("CP_" + to_string(n) + ".txt", ios::app);
        ofstream fileSS("SS_" + to_string(n) + ".txt", ios::app);
        fileCP << "MethodDuration,SearchTime,MeanTime,ConfidenceInterval_i,ConfidenceInterval_f" << endl;
        fileSS << "MethodDuration,SearchTime,MeanTime,ConfidenceInterval_i,ConfidenceInterval_f" << endl;
        
        // 100 iteraciones por cada potencia de 2
        for (int j = 0; j < 100; j++) {
            vector<Point> points = generatePoint(n);
            vector<Query> querys = generateQuerys();

            testCP(points, querys, fileCP);
            testSS(points, querys, fileSS);
        }
        fileCP.close();
        fileSS.close();
    }

    return 0;
}