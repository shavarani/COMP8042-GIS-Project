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
        result->insert(record_offset);
        index_size++;
    } else {
        Node a(pt);
        a.insert(record_offset);
        if(index.insert(&a))
            index_size++;
    }

}

set<int> CoordinateIndex::lookup_record(const DMS& latitude, const DMS& longitude) {
    Point pt(latitude, longitude);
    Node* result = index.search(pt);
    if (result != nullptr){
        return result->record_offsets;
    } else
        return std::set<int>{};
}

int CoordinateIndex::get_index_size() const {
    return index_size;
}

std::string CoordinateIndex::str() const {
    return index.str();
}

std::string CoordinateIndex::visualize() const {
    return index.visualize(40, 150);
}