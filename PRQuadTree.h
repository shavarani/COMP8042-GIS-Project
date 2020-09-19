//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#ifndef FINAL_PROJECT_PRQUADTREE_H
#define FINAL_PROJECT_PRQUADTREE_H

#include <cmath>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include "DMS.cpp"

struct Point {
    DMS latitude;
    DMS longitude;
    Point(DMS _lat, DMS _long) : latitude(std::move(_lat)), longitude(std::move(_long)) {}
    Point() : latitude(), longitude(){}
};

struct Node {
    Point pos;
    std::vector<int> record_offsets;
    bool allocated = false;

    explicit Node(Point _pos) {
        pos = std::move(_pos);
        allocated = true;
    }

    Node() = default;

    void insert(int _record_offset){
        record_offsets.insert(record_offsets.end(), _record_offset);
    }

    std::string print() const {
        std::ostringstream os;
        os.precision(5);
        os << "Point["<< pos.latitude.str() << "," << pos.longitude.str() << "] : |";
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
