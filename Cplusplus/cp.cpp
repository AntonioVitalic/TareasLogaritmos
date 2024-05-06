#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <memory>
#include "MTree.h"  // This should be the header file containing the MTree class definition.

// Assuming the Point structure is defined in the MTree.h file
// If not, you should define it or include it here.

std::vector<int> randomIndices(int k, int n) {
    std::vector<int> indices;
    while (indices.size() < static_cast<size_t>(k)) {
        int candidate = std::rand() % n;
        if (std::find(indices.begin(), indices.end(), candidate) == indices.end()) {
            indices.push_back(candidate);
        }
    }
    return indices;
}

double distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

std::shared_ptr<MTree> CP(std::vector<Point>& points) {
    const int B = 4096 / sizeof(Entry);  // Assuming size of Entry is correctly defined
    const int b = B / 2;

    if (points.size() <= B) {
        auto tree = std::make_shared<MTree>();
        for (const auto& point : points) {
            tree->insert(point);
        }
        return tree;
    }

    std::vector<std::vector<Point>> clusters;
    int k;
    do {
        k = std::min(B, static_cast<int>(points.size()) / B);
        std::vector<int> indices = randomIndices(k, points.size());

        clusters.assign(k, std::vector<Point>());

        for (const auto& point : points) {
            double minDist = std::numeric_limits<double>::max();
            int minIndex = 0;
            for (int i = 0; i < k; i++) {
                double dist = distance(point, points[indices[i]]);
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = i;
                }
            }
            clusters[minIndex].push_back(point);
        }

        for (int i = 0; i < k; i++) {
            if (clusters[i].size() < b) {
                clusters.erase(clusters.begin() + i);
                indices.erase(indices.begin() + i);
                i--;  // Adjust loop index after erase
                k--;  // Reduce number of clusters
            }
        }
    } while (k == 1);  // Repeat if there's only one cluster left

    std::vector<std::shared_ptr<MTree>> trees;
    for (auto& cluster : clusters) {
        trees.push_back(CP(cluster));
    }

    // Assuming further steps to combine these trees as per the algorithm description
    return nullptr;  // This should be replaced with the actual tree combining logic
}

int main() {
    std::vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
    auto result = CP(points);
    if (result) {
        std::cout << "Tree has " << result->size() << " entries." << std::endl;
    }
    return 0;
}
