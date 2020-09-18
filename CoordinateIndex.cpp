//
// Created by Hassan S. Shavarani
//

#include "CoordinateIndex.h"

void CoordinateIndex::updateWorld(const World& wd) {
    world = wd;
    Point topL(wd.get_north_lat_dec(), wd.get_east_long_dec());
    Point botR(wd.get_south_lat_dec(), wd.get_west_long_dec());
    index = PRQuadTree(topL,botR);
}

void CoordinateIndex::index_record(GISRecord &record, int record_offset){
    Node a(Point(record.get_primary_lat_dec(), record.get_primary_long_dec()), record_offset);
    index.insert(&a);
}

int CoordinateIndex::get_index_size() const {
    return index_size;
}