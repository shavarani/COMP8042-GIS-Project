//
// Created From https://www.geeksforgeeks.org/quad-tree/
//

#include "PRQuadTree.h"

#include <utility>

PRQuadTree::PRQuadTree(): bucket(){
    topLeft = Point();
    botRight = Point();
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
    topLeft = std::move(topL);
    botRight = std::move(botR);
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
    } else if (topLeft.latitude != botRight.latitude && topLeft.longitude != botRight.longitude){
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
    // '%' is averaging two DMS values
    if ((topLeft.latitude % botRight.latitude) >= p.latitude) {
        // Indicates topLeftTree
        if ((topLeft.longitude % botRight.longitude) >= p.longitude) {
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
        // '%' is averaging two DMS values
        if ((topLeft.longitude % botRight.longitude) >= p.longitude) {
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

std::string PRQuadTree::str(int level, const std::string& parent_prefix) const {
    std::ostringstream os;
    if (topLeftTree != nullptr)
        os << topLeftTree -> str(level + 1, parent_prefix + "   ");
    else if (!is_leaf_node())
        os << parent_prefix << "   *" << endl;

    if (topRightTree != nullptr)
        os << topRightTree -> str(level + 1, parent_prefix+ "   ");
    else if (!is_leaf_node())
        os << parent_prefix << "   *" << endl;

    if (bucket.empty())
        os << parent_prefix << "@" << std::endl;
    else {
        os << parent_prefix ;
        for (auto& n : bucket)
            os << n.str() << " ";
        os << endl;
    }
    if (botLeftTree != nullptr)
        os << botLeftTree -> str(level + 1, parent_prefix+ "   ");
    else if (!is_leaf_node())
        os << parent_prefix << "   *" << endl;

    if (botRightTree != nullptr)
        os << botRightTree -> str(level + 1, parent_prefix+ "   ");
    else if (!is_leaf_node())
        os << parent_prefix << "   *" << endl;
    return os.str();
}

PRQuadTree* PRQuadTree::expand_tree_for_node(Node* node) {
    // '%' is averaging two DMS values
    if ((topLeft.latitude % botRight.latitude) <= node->pos.latitude) {
        if ((topLeft.longitude % botRight.longitude) >= node->pos.longitude) {
            // Indicates topLeftTree
            if (topLeftTree == nullptr)
                topLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, topLeft.longitude),
                        Point((topLeft.latitude % botRight.latitude) ,(topLeft.longitude % botRight.longitude)));
            return topLeftTree;
        } else {
            // Indicates topRightTree
            if (botLeftTree == nullptr)
                botLeftTree = new PRQuadTree(
                        Point(topLeft.latitude, (topLeft.longitude % botRight.longitude)),
                        Point((topLeft.latitude % botRight.latitude), botRight.longitude));
            return botLeftTree;
        }
    }
    else {
        if ((topLeft.longitude % botRight.longitude) >= node->pos.longitude) {
            // Indicates botLeftTree
            if (topRightTree == nullptr)
                topRightTree = new PRQuadTree(
                        Point((topLeft.latitude % botRight.latitude), topLeft.longitude),
                        Point(botRight.latitude, (topLeft.longitude % botRight.longitude)));
            return topRightTree;
        } else {
            // Indicates botRightTree
            if (botRightTree == nullptr)
                botRightTree = new PRQuadTree(
                        Point((topLeft.latitude % botRight.latitude),(topLeft.longitude % botRight.longitude)),
                        Point(botRight.latitude, botRight.longitude));
            return botRightTree;
        }
    }
}

bool PRQuadTree::is_leaf_node() const{
    return topLeftTree == nullptr && topRightTree == nullptr && botLeftTree == nullptr && botRightTree == nullptr;
}
