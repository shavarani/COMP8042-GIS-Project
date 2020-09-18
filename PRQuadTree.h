//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#ifndef FINAL_PROJECT_PRQUADTREE_H
#define FINAL_PROJECT_PRQUADTREE_H

#include <cmath>

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
        record_offset = 0;
    }
};

class PRQuadTree {
    private:
    // Hold details of the boundary of this node
        Point topLeft;
        Point botRight;
        // Contains details of node
        Node *n;
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
};


#endif //FINAL_PROJECT_PRQUADTREE_H
