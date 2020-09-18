//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#include "PRQuadTree.h"

PRQuadTree::PRQuadTree(): n(){
    topLeft = Point(0, 0);
    botRight = Point(0, 0);
    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
}

PRQuadTree::PRQuadTree(Point topL, Point botR): n() {
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
    if (abs(topLeft.latitude - botRight.latitude) <= RESOLUTION
            && abs(topLeft.longitude - botRight.longitude) <= RESOLUTION) {
        if (!n.is_allocated())
            n = *node;
        return;
    }
    if ((topLeft.latitude + botRight.latitude) / 2.0 <= node->pos.latitude) {
        if ((topLeft.longitude + botRight.longitude) / 2 >= node->pos.longitude) {
            // Indicates topLeftTree
            if (topLeftTree == nullptr)
                topLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, topLeft.longitude),
                        Point((topLeft.latitude + botRight.latitude) / 2,(topLeft.longitude + botRight.longitude) / 2));
            topLeftTree->insert(node);
        } else {
            // Indicates topRightTree
            if (botLeftTree == nullptr)
                botLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, (topLeft.longitude + botRight.longitude) / 2),
                        Point((topLeft.latitude + botRight.latitude) / 2, botRight.longitude));
            botLeftTree->insert(node);
        }
    }
    else {
        if ((topLeft.longitude + botRight.longitude) / 2 >= node->pos.longitude) {
            // Indicates botLeftTree
            if (topRightTree == nullptr)
                topRightTree = new PRQuadTree(
                        Point((topLeft.latitude + botRight.latitude) / 2, topLeft.longitude),
                        Point(botRight.latitude, (topLeft.longitude + botRight.longitude) / 2));
            topRightTree->insert(node);
        } else {
            // Indicates botRightTree
            if (botRightTree == nullptr)
                botRightTree = new PRQuadTree(
                        Point((topLeft.latitude + botRight.latitude) / 2,(topLeft.longitude + botRight.longitude) / 2),
                        Point(botRight.latitude, botRight.longitude));
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
    if (n.is_allocated())
        return &n;
    if ((topLeft.latitude + botRight.latitude) / 2 >= p.latitude) {
        // Indicates topLeftTree
        if ((topLeft.longitude + botRight.longitude) / 2 >= p.longitude) {
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
        if ((topLeft.longitude + botRight.longitude) / 2 >= p.longitude) {
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
    return (p.latitude <= topLeft.latitude && p.latitude >= botRight.latitude &&
            p.longitude >= topLeft.longitude && p.longitude <= botRight.longitude);
}

std::string PRQuadTree::print() const {
    std::ostringstream os;
    if (n.is_allocated())
        os << n.print() << std::endl;
    if (topLeftTree != nullptr)
        os << topLeftTree -> print();
    if (topRightTree != nullptr)
        os << topRightTree -> print();
    if (botLeftTree != nullptr)
        os << botLeftTree -> print();
    if (botRightTree != nullptr)
        os << botRightTree -> print();
    return os.str();
}