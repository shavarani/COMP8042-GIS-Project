//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#include "PRQuadTree.h"

PRQuadTree::PRQuadTree(): bucket(){
    topLeft = Point(0, 0);
    botRight = Point(0, 0);
    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
}

PRQuadTree::PRQuadTree(Point topL, Point botR): bucket() {
    topLeftTree = nullptr;
    topRightTree = nullptr;
    botLeftTree = nullptr;
    botRightTree = nullptr;
    topLeft = topL;
    botRight = botR;
}

bool PRQuadTree::insert(Node *node) {
    if (node == nullptr)
        return false;
    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return false;
    if (!is_leaf_node()){
        return expand_tree_for_node(node)->insert(node);
    } else if (bucket.size() < BUCKET_SIZE){
        bucket.insert(bucket.end(), *node);
        return true;
    } else if (abs(topLeft.latitude - botRight.latitude) >= RESOLUTION
               && abs(topLeft.longitude - botRight.longitude) >= RESOLUTION){
        for (auto& n : bucket)
            expand_tree_for_node(&n)->insert(&n);
        bucket.clear();
        return expand_tree_for_node(node)->insert(node);
    } else
        return false;
}

// Find a node in a quadtree
Node* PRQuadTree::search(Point p) {
    // Current quad cannot contain it
    if (!inBoundary(p))
        return nullptr;
    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (!bucket.empty()) {
        for (auto& n : bucket){
            if (n.pos.latitude == p.latitude && n.pos.longitude == p.longitude)
                return &n;
        }
        return nullptr;
    }
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

std::string PRQuadTree::print(int level, const std::string& parent_prefix) const {
    std::ostringstream os;
    for (auto& n : bucket) {
        for (int i = 0; i < level-1; ++i) {
            os << "-";
        }
        os << "|" << parent_prefix << "|" << n.print();
    }
    if (topLeftTree != nullptr)
        os << topLeftTree -> print(level + 1, parent_prefix + "<NW>");
    if (topRightTree != nullptr)
        os << topRightTree -> print(level + 1, parent_prefix + "<NE>");
    if (botLeftTree != nullptr)
        os << botLeftTree -> print(level + 1, parent_prefix + "<SW>");
    if (botRightTree != nullptr)
        os << botRightTree -> print(level + 1, parent_prefix + "<SE>");
    return os.str();
}

PRQuadTree* PRQuadTree::expand_tree_for_node(Node* node) {
    if ((topLeft.latitude + botRight.latitude) / 2.0 <= node->pos.latitude) {
        if ((topLeft.longitude + botRight.longitude) / 2 >= node->pos.longitude) {
            // Indicates topLeftTree
            if (topLeftTree == nullptr)
                topLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, topLeft.longitude),
                        Point((topLeft.latitude + botRight.latitude) / 2,(topLeft.longitude + botRight.longitude) / 2));
            return topLeftTree;
        } else {
            // Indicates topRightTree
            if (botLeftTree == nullptr)
                botLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, (topLeft.longitude + botRight.longitude) / 2),
                        Point((topLeft.latitude + botRight.latitude) / 2, botRight.longitude));
            return botLeftTree;
        }
    }
    else {
        if ((topLeft.longitude + botRight.longitude) / 2 >= node->pos.longitude) {
            // Indicates botLeftTree
            if (topRightTree == nullptr)
                topRightTree = new PRQuadTree(
                        Point((topLeft.latitude + botRight.latitude) / 2, topLeft.longitude),
                        Point(botRight.latitude, (topLeft.longitude + botRight.longitude) / 2));
            return topRightTree;
        } else {
            // Indicates botRightTree
            if (botRightTree == nullptr)
                botRightTree = new PRQuadTree(
                        Point((topLeft.latitude + botRight.latitude) / 2,(topLeft.longitude + botRight.longitude) / 2),
                        Point(botRight.latitude, botRight.longitude));
            return botRightTree;
        }
    }
}

bool PRQuadTree::is_leaf_node() {
    return topLeftTree == nullptr && topRightTree == nullptr && botLeftTree == nullptr && botRightTree == nullptr;
}
