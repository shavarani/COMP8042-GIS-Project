//
// Created by Hassan S. Shavarani
//

#include "CoordinateIndex.h"

#include <utility>

void CoordinateIndex::updateWorld(const World& wd) {
    Point topL(wd.get_north_lat_dms(), wd.get_west_long_dms());
    Point botR(wd.get_south_lat_dms(), wd.get_east_long_dms());
    index = PRQuadTree(topL,botR);
}

void CoordinateIndex::index_record(DMS latitude, DMS longitude, int record_offset){
    Point pt(std::move(latitude), std::move(longitude));
    Node* result = index.search(pt);
    if (result != nullptr){
        throw std::invalid_argument("This has never happened!");
        // result->insert(record_offset);
        // index_size++;
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

std::string CoordinateIndex::str() const {
    return index.str();
}