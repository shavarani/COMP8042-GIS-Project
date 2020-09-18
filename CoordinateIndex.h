//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_COORDINATEINDEX_H
#define FINAL_PROJECT_COORDINATEINDEX_H

#include "PRQuadTree.h"
#include "World.h"

class CoordinateIndex {
    // Will use a bucket PR quadtree for the physical organization.
    //		in which each leaf stores up to K data objects (here K=4)
    // The PR quadtree implementation should follow good design practices,
    //      and its interface should be somewhat similar to that of the BST.

    // The index entries held in the quadtree will store
    //		1. a geographic coordinate
    // 		2. a collection of the file offsets of the matching GIS records in the database file.

    // Quadtree implementation notes:
    // You are expected to implement different types for the leaf and internal nodes,
    //		with appropriate data membership for each,
    //	and an abstract base type from which they are both derived.

    // You must be able to display the PR quadtree in a readable manner.
    //		The display must clearly indicate the structure of the tree,
    //		the relationships between its nodes, and the data objects in the leaf nodes.

    //Quadtree children are printed in the order SW  SE  NE  NW
    private:
        int index_size = 0.0;
        PRQuadTree index;
    public:
        ~CoordinateIndex() = default;
        CoordinateIndex() = default;
        CoordinateIndex( const CoordinateIndex & rhs ) = default; // Copy Constructor
        CoordinateIndex( CoordinateIndex && rhs ) = default; // Move Constructor
        CoordinateIndex & operator= ( const CoordinateIndex & rhs ) = default; // Copy Assignment
        CoordinateIndex & operator= ( CoordinateIndex && rhs ) = default; // Move Assignment

        void updateWorld(const World& wd);

        void index_record(double latitude, double longitude, int record_offset);

        int get_index_size() const;

        std::string print() const;
};


#endif //FINAL_PROJECT_COORDINATEINDEX_H
