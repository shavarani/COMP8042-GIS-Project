//
// Created by Hassan S. Shavarani
//

#include "World.h"


World::World() {
    west_long_dms = east_long_dms = south_lat_dms = north_lat_dms = DMS();
}

World::World(const string& west_long, const string& east_long, const string& south_lat, const string& north_lat):
    west_long_dms(west_long), east_long_dms(east_long), south_lat_dms(south_lat), north_lat_dms(north_lat){
    //			- features that lie outside the specified coordinate space simply will not be indexed.
    /*
        Quadtree children are printed in the order SW  SE  NE  NW
        --------------------------------------------------------------------------------

        Latitude/longitude values in index entries are shown as signed integers, in total seconds.

        World boundaries are set to:
                      138600
           -396000                -367200
                      109800
        --------------------------------------------------------------------------------
     * */
}

bool World::is_in_world_boundaries(GISRecord & record) const{
    if (record.get_primary_lat_dms().get_direction() == NULL_ISLAND ||
            record.get_primary_long_dms().get_direction() == NULL_ISLAND)
        return false;
    bool lat_in =  south_lat_dms < record.get_primary_lat_dms() && record.get_primary_lat_dms() < north_lat_dms;
    bool long_in = west_long_dms < record.get_primary_long_dms() && record.get_primary_long_dms() < east_long_dms;
    return  lat_in && long_in;
}

DMS World::get_west_long_dms() const{
    return west_long_dms;
}
DMS World::get_east_long_dms() const{
    return east_long_dms;
}
DMS World::get_south_lat_dms() const{
    return south_lat_dms;
}
DMS World::get_north_lat_dms() const{
    return north_lat_dms;
}

string World::str() {
    string log_tabs = "\t\t\t\t\t\t";
    std::ostringstream os;
    os  << "------------------------------------------------------------------------------------------" << endl
        << "Latitude/longitude values in index entries are shown as signed integers, in total seconds." <<endl
        << "------------------------------------------------------------------------------------------" << endl
        <<  log_tabs << "World boundaries are set to:"<< endl
        <<  log_tabs << "           " << north_lat_dms.str() << endl
        <<  log_tabs << west_long_dms.str() <<  "              " << east_long_dms.str() << endl
        <<  log_tabs << "           " << south_lat_dms.str() << endl
        << "------------------------------------------------------------------------------------------" << endl;
    return os.str();
}