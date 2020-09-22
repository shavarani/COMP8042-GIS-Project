//
// Created by Hassan S. Shavarani
//

#include "SystemManager.h"

void SystemManager::record_to_db(const string & raw_record){
    std::ostringstream os;
    os << raw_record << endl;
    string res = os.str();
    pool.get_db_file()->write_to_file(res);
    last_db_record_offset += 1;
}

SystemManager::SystemManager(const char* db_file_adr): pool(), world(), n_index(), c_index(), last_db_record_offset(0){
    pool.create_db_file(db_file_adr);
}

string SystemManager::process_world_command(const string& west_long, const string& east_long,
                                 const string& south_lat, const string& north_lat){
    // 		3. World command (occur once):
    //			- The first non-comment line will specify the world boundaries (in DMS format) to be used:
    //			- world<tab><westLong><tab><eastLong><tab><southLat><tab><northLat>
    world = World(west_long, east_long, south_lat, north_lat);
    c_index.updateWorld(world);
    return world.str();
}

string SystemManager::process_import_command(const string & gis_record_file_name){
    //		4. Import command:
    //			- import<tab><GIS record file>
    //			- Add all the valid GIS records in the specified file to the database file.
    //				- records will be appended to the existing database file
    //				- those records will be indexed in the manner described earlier.
    //			- When completed, log
    //				- the number of entries added to each index,
    //				- the longest probe sequence that was needed when inserting to the hash table.
    vector<string> gis_records = FileManager::read_file("../GIS_FILES/"+gis_record_file_name);
    bool first_record_seen = false;
    int n_index_size_before_import = n_index.get_index_size();
    int c_index_size_before_import = c_index.get_index_size();
    for (const auto& gis_record: gis_records) {
        if (first_record_seen) {
            GISRecord record(gis_record);
            if(world.is_in_world_boundaries(record)){
                n_index.index_record(record.get_feature_name(), record.get_state_alpha(), last_db_record_offset);
                c_index.index_record(record.get_primary_lat_dms(), record.get_primary_long_dms(), last_db_record_offset);
                record_to_db(gis_record);
            }
        }
        first_record_seen = true;
    }
    std::ostringstream os;
    os  << "Imported Features by name: " << n_index.get_index_size() - n_index_size_before_import << endl
        << "Longest probe sequence:    " << n_index.get_longest_probe_sequence() << endl
        << "Imported Locations:        " << c_index.get_index_size() - c_index_size_before_import << endl
        << "Average name length:       " << n_index.get_average_name_length() << endl;
    return os.str();
}

// TODO		-- optional -sort flag for "what is" commands needs to be worked out!
string SystemManager::process_what_is_command(const string & feature_name, const string & state_abbreviation){
    //		7. what is command
    //			- what is<tab><feature name><tab><state abbreviation>
    //			- find every GIS record in the database file that matches the given <feature name> and <state abbreviation>
    std::set<int> possible_offsets = n_index.lookup_record(feature_name, state_abbreviation);
    if (possible_offsets.empty())
        return "  No records match \""+feature_name+"\" and \""+state_abbreviation+"\"\n";
    vector<GISRecord> res = pool.retrieve_records(possible_offsets);
    std::ostringstream os;
    //			- for each record log only
    //				- the offset (at which the record was found)
    //				- the county name
    //				- the primary latitude
    //				- the primary longitude
    for (auto& elem :res)
        os  << "  " << elem.get_file_offset() << ":  "
            << elem.get_county_name() << "  ("
            << elem.get_primary_lat_dms().str() << ", "
            << elem.get_primary_long_dms().str() << ")" << endl;
    return os.str();
}


// TODO		-- optional -sort flag for "what is at" commands needs to be worked out!
string SystemManager::process_what_is_at_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long){
    //		8. what is at command
    //			- what is at<tab><geographic coordinate>
    //			- find every GIS record in the database file that matches the given <geographic coordinate>,
    DMS gc_lat(geographic_coordinate_lat);
    DMS gc_long(geographic_coordinate_long);
    std::set<int> possible_offsets = c_index.lookup_record(gc_lat, gc_long);
    if (possible_offsets.empty())
        return "  Nothing was found at \""+gc_lat.str()+"\" and \""+gc_long.str()+"\"\n";
    vector<GISRecord> res = pool.retrieve_records(possible_offsets);
    std::ostringstream os;
    //				- for each record log only log:
    //					- the offset (at which the record was found)
    //					- the feature name
    //					- county name
    //					- state abbreviation
    os << "  The following feature(s) were found at (" << gc_lat.str() << ", " << gc_long.str() << ")" << endl;
    for (auto& elem :res)
        os  << "\t" << elem.get_file_offset() << ":  \""
            << elem.get_feature_name() << "\"  \""
            << elem.get_county_name() << "\"  \""
            << elem.get_state_alpha() << "\"" << endl;
    return os.str();
}

// TODO		-- optional -sort flag for "what is in" commands needs to be worked out!
string SystemManager::process_what_is_in_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long,
                                                 int half_height, int half_width, const string & filter, bool long_report){
    //		9. What is in command
    //			- what is in<tab><geographic coordinate><tab><half-height><tab><half-width>
    //				- optional -long : display of a long listing of the relevant records
    //									- The switch will be the first token following the name of the command.
    //				- optional -filter [ pop | water | structure ]
    //									- The switch and its modifier will be the first and second tokens following the name of the command.
    //			- find every GIS record in the database file whose coordinates fall within the closed rectangle
    //					(with the specified height and width [specified as seconds], centered at the <geographic coordinate>)
    //				- if [-filter] provided: filter the set of matching records to only show those whose feature type field corresponds to the given filter specifier (Table 2 in the Appendix).
    //					- if [-long] provided: for each record log every important non-empty field, nicely formatted and labeled.
    //					- else: for each record only log
    //						- the offset (at which the record was found)
    //						- the feature name
    //						- the state name
    //						- the primary latitude
    //						- the primary longitude
    DMS gc_lat(geographic_coordinate_lat);
    DMS gc_long(geographic_coordinate_long);
    std::set<int> possible_offsets = c_index.lookup_area(gc_lat, gc_long, half_width, half_height);
    if (possible_offsets.empty())
        return "  Nothing was found in \""+gc_lat.str()+" +/- "+to_string(half_height)+"\" and \""+gc_long.str()+" +/- "+to_string(half_width)+"\"\n";
    vector<GISRecord> res = pool.retrieve_records(possible_offsets);
    std::ostringstream os;
    if(filter.empty()) {
        os << "  The following " << res.size() << " feature(s) were found in (" << gc_lat.str() << " +/- "
           << half_height << ", " << gc_long.str() << " +/- " << half_width << ")" << endl;
    } else {
        os << "  The following features matching your criteria were found in  (" << gc_lat.str() << " +/- "
           << half_height << ", " << gc_long.str() << " +/- " << half_width << ")" << endl;
    }
    int filter_cnt = 0;
    if (long_report) {
        for (auto &elem :res) {
            if (!filter.empty() && filters.find(filter)->second.count(elem.get_feature_class()) == 0) {
                continue;
            }
            filter_cnt++;
            os << "  Feature ID   : " << elem.get_feature_id() << endl
               << "  Feature Name : " << elem.get_feature_name() << endl
               << "  Feature Cat  : " << elem.get_feature_class() << endl
               << "  State        : " << elem.get_state_alpha() << endl
               << "  County       : " << elem.get_county_name() << endl
               << "  Longitude    : " << elem.get_primary_long_dms().str() << endl
               << "  Latitude     : " << elem.get_primary_lat_dms().str() << endl
               << "  Elev in ft   : " << elem.get_elevation_ft() << endl
               << "  USGS Quad    : " << elem.get_map_name() << endl
               << "  Date created : " << elem.get_date_created() << endl;
        }
    } else {
        for (auto &elem :res) {
            if (!filter.empty() && filters.find(filter)->second.count(elem.get_feature_class()) == 0) {
                continue;
            }
            filter_cnt++;
            os << "\t" << elem.get_file_offset() << ":  \""
               << elem.get_feature_name() << "\"  \""
               << elem.get_state_alpha() << "\"  \""
               << "(" << gc_lat.str() << ", " << gc_long.str() << ")" << endl;
        }
    }
    if(!filter.empty()){
        os << endl <<"  There were "<<filter_cnt<<" features of type "<<filter <<"." << endl;
    }
    return os.str();
}

string SystemManager::process_debug_command(const string & component_name){
    //		5. Debug command:
    //			- debug<tab>[ quad | hash | pool | world]
    //			- Log the contents of the specified index structure
    //				- include key values and file offsets
    if (component_name == "world") {
        return c_index.visualize();
    } else if (component_name == "quad") {
        return c_index.str();
    } else if (component_name == "hash") {
        return n_index.str();
    } else if (component_name == "pool") {
        cout << "debugging " + component_name << endl;
    } else {
        throw std::invalid_argument("Invalid component_name: "+component_name);
    }
    return "";
}