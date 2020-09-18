//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#ifndef FINAL_PROJECT_PRQUADTREE_H
#define FINAL_PROJECT_PRQUADTREE_H

#include <cmath>
#include <string>
#include <sstream>
#include <vector>

struct Point {
    double latitude;
    double longitude;
    Point(double _lat, double _long) {
        latitude = _lat;
        longitude = _long;
    }
    Point() {
        latitude = 0;
        longitude = 0;
    }
};

struct Node {
    Point pos;
    std::vector<int> record_offsets;
    bool allocated = false;

    explicit Node(Point _pos) {
        pos = _pos;
        allocated = true;
    }

    Node() = default;

    void insert(int _record_offset){
        record_offsets.insert(record_offsets.end(), _record_offset);
    }

    std::string print() const {
        std::ostringstream os;
        os.precision(5);
        os << "Point["<< pos.latitude << "," << pos.longitude << "] : |";
        for(auto& record_offset: record_offsets)
            os << record_offset << "|";
        os << std::endl;
        return os.str();
    }
};

class PRQuadTree {
    private:
    // Hold details of the boundary of this node
        // The minimum resolution is one second of arc.
        constexpr static double RESOLUTION = 1 / 3600.0;
        const static int BUCKET_SIZE = 4;
        Point topLeft;
        Point botRight;
        // Contains details of node
        std::vector<Node> bucket;
        // Children of this tree
        PRQuadTree *topLeftTree;
        PRQuadTree *topRightTree;
        PRQuadTree *botLeftTree;
        PRQuadTree *botRightTree;
        PRQuadTree* expand_tree_for_node(Node*);
        bool is_leaf_node();

    public:
        PRQuadTree();
        PRQuadTree(Point topL, Point botR);
        bool insert(Node*);
        Node* search(Point);
        bool inBoundary(Point);
        std::string print(int level = 0, const std::string& parent_prefix = "") const;
};


#endif //FINAL_PROJECT_PRQUADTREE_H
