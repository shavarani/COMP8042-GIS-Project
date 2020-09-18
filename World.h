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
        string west_long_dms;
        string east_long_dms;
        string south_lat_dms;
        string north_lat_dms;
        double west_long_dec;
        double east_long_dec;
        double south_lat_dec;
        double north_lat_dec;
    public:
        explicit World();
        World( const World & rhs ) = default; // Copy Constructor
        World( World && rhs ) = default; // Move Constructor
        World & operator= ( const World & rhs ) = default; // Copy Constructor
        World & operator= ( World && rhs ) = default; // Move Constructor
        World(const string& west_long, const string& east_long, const string& south_lat, const string& north_lat);
        string print();
        bool is_in_world_boundaries(GISRecord & record) const;
};


#endif //FINAL_PROJECT_WORLD_H
