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
