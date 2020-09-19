//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_WORLD_H
#define FINAL_PROJECT_WORLD_H

#include <stdexcept>
#include <sstream>
#include "GISRecord.h"

using namespace std;

class World {
    private:
        DMS west_long_dms;
        DMS east_long_dms;
        DMS south_lat_dms;
        DMS north_lat_dms;
    public:
        explicit World();
        World( const World & rhs ) = default; // Copy Constructor
        World( World && rhs ) = default; // Move Constructor
        World & operator= ( const World & rhs ) = default; // Copy Constructor
        World & operator= ( World && rhs ) = default; // Move Constructor
        World(const string& west_long, const string& east_long, const string& south_lat, const string& north_lat);
        string str();
        bool is_in_world_boundaries(GISRecord & record) const;
        DMS get_west_long_dms() const;
        DMS get_east_long_dms() const;
        DMS get_south_lat_dms() const;
        DMS get_north_lat_dms() const;
};


#endif //FINAL_PROJECT_WORLD_H
