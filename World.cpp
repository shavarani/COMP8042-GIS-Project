//
// Created by Hassan S. Shavarani
//

#include "World.h"


double convert_dms_to_dec(const string& dms) {
    if (dms.length()!= 7 && dms.length() != 8){
        throw std::invalid_argument("DMS values are expected to be of sizes 7 or 8");
    }
    bool neg(false) ;
    bool is_latitude(false);
    if (dms[dms.length() - 1] == 'W'){
        neg = true;
    } else if (dms[dms.length() - 1] == 'S') {
        is_latitude = true;
        neg = true;
    } else if (dms[dms.length() - 1] == 'N') {
        is_latitude = true;
    } else if (dms[0] == '-') {
        throw std::invalid_argument("DMS values are expected to show the sign using the direction modifiers at the end");
    }
    double deg, min_, sec;
    deg = stof(dms.substr(0, is_latitude? 2: 3));
    min_= stof(dms.substr(is_latitude? 2: 3, 2));
    sec = stof(dms.substr(is_latitude? 4: 5, 2));
    double ang = deg + ((min_ + (sec / 60.0)) / 60.0) ;
    return neg? -ang: ang;
}

World::World() {
    west_long_dms = east_long_dms = south_lat_dms = north_lat_dms = "";
    west_long_dec = east_long_dec = south_lat_dec = north_lat_dec = 0.0;
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
    west_long_dec = convert_dms_to_dec(west_long);
    east_long_dec = convert_dms_to_dec(east_long);
    south_lat_dec = convert_dms_to_dec(south_lat);
    north_lat_dec = convert_dms_to_dec(north_lat);
}

bool World::is_in_world_boundaries(GISRecord & record) const{
    bool lat_in =  south_lat_dec < record.get_primary_lat_dec() && record.get_primary_lat_dec() < north_lat_dec;
    bool long_in = west_long_dec < record.get_primary_long_dec() && record.get_primary_long_dec() < east_long_dec;
    return  lat_in && long_in;
}

string World::print() {
    string log_tabs = "\t\t\t\t\t\t";
    std::ostringstream os;
    os  << "------------------------------------------------------------------------------------------" << endl
        << "Latitude/longitude values in index entries are shown as signed integers, in total seconds." <<endl
        << "------------------------------------------------------------------------------------------" << endl
        <<  log_tabs << "World boundaries are set to:"<< endl
        <<  log_tabs << "           " << (north_lat_dec < 0? "-": "") << north_lat_dms.substr(0, 6) << endl
        <<  log_tabs << (west_long_dec < 0? "-": "") << west_long_dms.substr(0, 6) << "              "
        << (east_long_dec < 0? "-": "") << east_long_dms.substr(0, 6) << endl
        <<  log_tabs << "           " << (south_lat_dec < 0? "-": "") << south_lat_dms.substr(0, 6) << endl
        << "------------------------------------------------------------------------------------------" << endl;
    return os.str();
}