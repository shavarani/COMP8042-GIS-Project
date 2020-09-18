//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#include "PRQuadTree.h"

PRQuadTree::PRQuadTree() {
    topLeft = Point(0, 0);
    botRight = Point(0, 0);
    n = nullptr;
    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
}

PRQuadTree::PRQuadTree(Point topL, Point botR) {
    n = nullptr;
    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
    topLeft = topL;
    botRight = botR;
}

void PRQuadTree::insert(Node *node) {
    if (node == nullptr)
        return;
    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;
    // We are at a quad of unit area
    // We cannot subdivide this quad further
    if (abs(topLeft.x - botRight.x) <= 1 &&
        abs(topLeft.y - botRight.y) <= 1) {
        if (n == nullptr)
            n = node;
        return;
    }
    if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
            if (topLeftTree == nullptr)
                topLeftTree = new PRQuadTree(
                        Point(topLeft.x, topLeft.y),
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2));
            topLeftTree->insert(node);
        } else { // Indicates botLeftTree
            if (botLeftTree == nullptr)
                botLeftTree = new PRQuadTree(
                        Point(topLeft.x,
                              (topLeft.y + botRight.y) / 2),
                        Point((topLeft.x + botRight.x) / 2,
                              botRight.y));
            botLeftTree->insert(node);
        }
    }
    else {
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y) { // Indicates topRightTree
            if (topRightTree == nullptr)
                topRightTree = new PRQuadTree(
                        Point((topLeft.x + botRight.x) / 2,
                              topLeft.y),
                        Point(botRight.x,
                              (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        } else { // Indicates botRightTree
            if (botRightTree == nullptr)
                botRightTree = new PRQuadTree(
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2),
                        Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}

// Find a node in a quadtree
Node* PRQuadTree::search(Point p) {
    // Current quad cannot contain it
    if (!inBoundary(p))
        return nullptr;
    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != nullptr)
        return n;
    if ((topLeft.x + botRight.x) / 2 >= p.x) {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topLeftTree == nullptr)
                return nullptr;
            return topLeftTree->search(p);
        } else {// Indicates botLeftTree
            if (botLeftTree == nullptr)
                return nullptr;
            return botLeftTree->search(p);
        }
    }
    else {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y) {
            if (topRightTree == nullptr)
                return nullptr;
            return topRightTree->search(p);
        } else { // Indicates botRightTree
            if (botRightTree == nullptr)
                return nullptr;
            return botRightTree->search(p);
        }
    }
};

// Check if current quadtree contains the point
bool PRQuadTree::inBoundary(Point p) {
    return (p.x >= topLeft.x &&
            p.x <= botRight.x &&
            p.y >= topLeft.y &&
            p.y <= botRight.y);
}