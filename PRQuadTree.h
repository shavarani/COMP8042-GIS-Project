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
#include <set>
#include "DMS.cpp"

struct Point {
    DMS latitude;
    DMS longitude;
    Point(DMS _lat, DMS _long) : latitude(std::move(_lat)), longitude(std::move(_long)) {}
    Point() : latitude(), longitude(){}
};

struct Node {
    Point pos;
    std::set<int> record_offsets;
    bool allocated = false;

    explicit Node(Point _pos) {
        pos = std::move(_pos);
        allocated = true;
    }

    Node() = default;

    Node & operator= ( const Node & rhs ) = default;

    void insert(int _record_offset){
        record_offsets.insert(record_offsets.end(), _record_offset);
    }

    std::string str() const {
        std::ostringstream os;
        os.precision(5);
        os << "[("<< pos.latitude.total_seconds() << "," << pos.longitude.total_seconds() << "), ";
        for(auto& record_offset: record_offsets)
            os << record_offset << "]";
        return os.str();
    }
};

class PRQuadTree {
    private:
    // Hold details of the boundary of this node
        // The minimum resolution is one second of arc.
        const static int BUCKET_SIZE = 4;
        int tree_size = 0;
        Point topLeft;
        Point botRight;
        // Contains details of node
        std::vector<Node> bucket;
        // Children of this tree
        PRQuadTree *botLeftTree;
        PRQuadTree *topLeftTree;
        PRQuadTree *botRightTree;
        PRQuadTree *topRightTree;
        PRQuadTree* expand_tree_for_node(Node*);
        bool is_leaf_node() const;
        void visualize_rcr(int visualization_matrix[], int row_start, int row_end,
                           int col_start, int col_end, int array_rows, int array_cols) const;

    public:
        PRQuadTree();
        PRQuadTree(Point topL, Point botR);
        bool insert(Node*);
        Node* search(Point);
        bool inBoundary(Point) const;
        std::string str(int level = 0, const std::string& parent_prefix = "") const;
        std::string visualize(int cell_rows, int cell_cols) const;
        void collected_rectangular_features(PRQuadTree& search_area);
        std::vector<Node> get_bucket();
        Point get_top_left_boundary();
        Point get_bottom_right_boundary();
};


#endif //FINAL_PROJECT_PRQUADTREE_H
