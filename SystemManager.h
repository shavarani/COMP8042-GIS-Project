//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_SYSTEMMANAGER_H
#define FINAL_PROJECT_SYSTEMMANAGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include "World.h"
#include "NameIndex.h"
#include "CoordinateIndex.h"
#include "BufferPool.h"


class SystemManager {
    // Use cases:
        // 		1. validates the command line arguments
        // 		2. manages the initialization of the various system components.
    private:
        int last_db_record_offset;
        World world;
        NameIndex n_index;
        CoordinateIndex c_index;
        void record_to_db(const string & raw_record);
        BufferPool pool;
        const std::map<std::string, std::set<string>> filters =
                {{"structure", {"Airport", "Bridge", "Building", "Church", "Dam", "Hospital", "Levee", "Park",
                                        "Post Office", "School", "Tower", "Tunnel"}},
                 {"water", {"Arroyo", "Bay", "Bend", "Canal", "Channel", "Falls","Glacier", "Gut", "Harbor",
                                        "Lake", "Rapids", "Reservoir", "Sea", "Spring", "Stream", "Swamp", "Well"}},
                 {"pop", {"Populated Place"}}};
    public:
        explicit SystemManager(const char* db_file_adr = nullptr);
        SystemManager( const SystemManager & rhs ) = delete; // Copy Constructor
        SystemManager( SystemManager && rhs ) = delete; // Move Constructor
        SystemManager & operator= ( const SystemManager & rhs ) = delete; // Copy Assignment
        SystemManager & operator= ( SystemManager && rhs ) = delete; // Move Assignment
        ~SystemManager() = default;
        string process_debug_command(const string & component_name);
        string process_world_command(const string& west_long, const string& east_long,
                                     const string& south_lat, const string& north_lat);
        string process_import_command(const string & gis_record_file_name);
        string process_what_is_command(const string & feature_name, const string & state_abbreviation);
        string process_what_is_at_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long);
        string process_what_is_in_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long,
                                          int half_height, int half_width, const string & filter, bool long_report);
};


#endif //FINAL_PROJECT_SYSTEMMANAGER_H
