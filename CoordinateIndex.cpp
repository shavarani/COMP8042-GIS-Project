//
// Created by Hassan S. Shavarani
//

#include "CoordinateIndex.h"

void CoordinateIndex::updateWorld(const World& wd) {
    Point topL(wd.get_north_lat_dec(), wd.get_west_long_dec());
    Point botR(wd.get_south_lat_dec(), wd.get_east_long_dec());
    index = PRQuadTree(topL,botR);
}

void CoordinateIndex::index_record(double latitude, double longitude, int record_offset){
    Point pt(latitude, longitude);
    Node* result = index.search(pt);
    if (result != nullptr){
        result->insert(record_offset);
        index_size++;
    } else {
        Node a(pt);
        a.insert(record_offset);
        if(index.insert(&a))
            index_size++;
    }

}

int CoordinateIndex::get_index_size() const {
    return index_size;
}

std::string CoordinateIndex::print() const {
    return index.print();
}