//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#ifndef FINAL_PROJECT_PRQUADTREE_H
#define FINAL_PROJECT_PRQUADTREE_H

#include <cmath>
#include <string>
#include <sstream>

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
    int record_offset;

    Node(Point _pos, int _record_offset) {
        pos = _pos;
        record_offset = _record_offset;
    }

    Node() {
        record_offset = -1;
    }

    bool is_allocated() const {
        return record_offset != -1;
    }

    std::string print() const {
        std::ostringstream os;
        os.precision(5);
        os << "Point["<< pos.latitude << "," << pos.longitude << "] : " << record_offset << std::endl;
        return os.str();
    }
};

class PRQuadTree {
    private:
    // Hold details of the boundary of this node
        // The minimum resolution is one second of arc.
        constexpr static double RESOLUTION = 1 / 3600.0;
        Point topLeft;
        Point botRight;
        // Contains details of node
        Node n;
        // Children of this tree
        PRQuadTree *topLeftTree;
        PRQuadTree *topRightTree;
        PRQuadTree *botLeftTree;
        PRQuadTree *botRightTree;

    public:
        PRQuadTree();
        PRQuadTree(Point topL, Point botR);
        void insert(Node*);
        Node* search(Point);
        bool inBoundary(Point);
        std::string print() const;
};


#endif //FINAL_PROJECT_PRQUADTREE_H
