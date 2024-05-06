#include <iostream>
#include <vector>
#include <memory>
#include "MTree.h"  // Assuming MTree.h contains the required class definitions

// Assuming distance is a function declared in MTree.h or another appropriate header
double distance(const Point& p1, const Point& p2);

void searchMTree(const std::shared_ptr<MTree>& mtree, const Query& query, std::vector<Point>& results) {
    const Point& q = query.p;
    const double r = query.r;

    for (const auto& entry : mtree->entries) {
        const Point& p = entry->p;
        const double dist = distance(p, q);
        if (entry->cr == 0) {  // Leaf node check
            if (dist <= r) {
                results.push_back(p);
            }
        } else {  // Internal node
            if (dist <= r + entry->cr) {
                if (entry->a) {
                    searchMTree(entry->a, query, results);
                }
            }
        }
    }
}

int main() {
    // Setup for an MTree and a Query
    std::shared_ptr<MTree> tree = std::make_shared<MTree>();
    // Populate tree with data
    tree->insert(Point(0.1, 0.2), 0.05);
    tree->insert(Point(0.3, 0.4), 0.06);

    Query query{Point(0.15, 0.25), 0.1};

    std::vector<Point> results;
    searchMTree(tree, query, results);

    std::cout << "Found " << results.size() << " entries within the search radius." << std::endl;
    for (const auto& result : results) {
        std::cout << "Point (" << result.x << ", " << result.y << ")" << std::endl;
    }

    return 0;
}
