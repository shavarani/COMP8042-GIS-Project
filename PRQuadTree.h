//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#ifndef FINAL_PROJECT_PRQUADTREE_H
#define FINAL_PROJECT_PRQUADTREE_H

#include <cmath>

struct Point {
    int x;
    int y;
    Point(int _x, int _y) {
        x = _x;
        y = _y;
    }
    Point() {
        x = 0;
        y = 0;
    }
};

struct Node {
    Point pos;
    int data;
    Node(Point _pos, int _data) {
        pos = _pos;
        data = _data;
    }
    Node() {
        data = 0;
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
