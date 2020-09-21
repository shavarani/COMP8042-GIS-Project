//
// Created by Hassan S. Shavarani
//

#include "GISRecord.h"

void alter_print_str(const string & name, const string & value, string & result,
                            bool & empty_found, const string & delim = " --- "){
    if (!value.empty()){
        result += delim + name + value;
    } else{
        empty_found = true;
    }
}


GISRecord::GISRecord(const string & raw_record){
    if(raw_record.empty()){
        return;
    }
    vector<string> gis_record_tokens;
    split(raw_record, gis_record_tokens, '|');
    auto itr = gis_record_tokens.begin();
    feature_id = std::stoi(*itr++);
    feature_name = *itr++;
    feature_class = *itr++;
    state_alpha = *itr++;
    state_numeric = *itr++;
    county_name = *itr++;
    county_numeric = *itr++;
    primary_lat_dms = DMS(*itr++);
    primary_long_dms = DMS(*itr++);
    string plad = *itr++;
    string plod = *itr++;
    //Note: Records showing "Unknown" and zeros for the latitude and longitude DMS and decimal fields,
    //respectively, indicate that the coordinates of the feature are unknown. They are recorded in the
    //database as zeros to satisfy the format requirements of a numerical data type. They are not errors and
    //do not reference the actual geographic coordinates at 0 latitude, 0 longitude.
    primary_lat_dec = plad.empty()? 0.0 : std::stof(plad);
    primary_long_dec = plod.empty()? 0.0 : std::stof(plod);
    string ladms = *itr++;
    string lodms = *itr++;
    source_lat_dms = ladms.empty()? DMS(): DMS(ladms);
    source_long_dms = lodms.empty()? DMS():DMS(lodms);
    string slad = *itr++;
    string slod = *itr++;
    source_lat_dec = slad.empty()? 0.0 : std::stof(slad);
    source_long_dec = slod.empty()? 0.0 : std::stof(slod);
    string eim = *itr++;
    string eif = *itr++;
    elev_in_m = eim.empty()? -std::numeric_limits<int>::max(): std::stoi(eim);
    elev_in_ft = eif.empty()? -std::numeric_limits<int>::max(): std::stoi(eif);
    map_name = *itr++;
    date_created = *itr++;
    date_edited = *itr++;
}

DMS GISRecord::get_primary_lat_dms() const{
    return primary_lat_dms;
}

DMS GISRecord::get_primary_long_dms() const{
    return primary_long_dms;
}

string GISRecord::get_feature_name() const{
    return feature_name;
}

string GISRecord::get_state_alpha() const{
    return state_alpha;
}

void GISRecord::set_file_offset(int offset){
    file_offset = offset;
}

int GISRecord::get_file_offset() const{
    return file_offset;
}

std::string GISRecord::get_county_name(){
    return county_name;
}

string GISRecord::str(){
    bool empty_found = false;
    string result;
    alter_print_str("Feature Id: ", to_string(feature_id), result, empty_found);
    alter_print_str("Feature Name: ", feature_name, result, empty_found);
    alter_print_str("Feature Class: ", feature_class, result, empty_found);
    alter_print_str("State Alpha: ", state_alpha, result, empty_found);
    alter_print_str("State Numeric: ", state_numeric, result, empty_found);
    alter_print_str("County Name: ", county_name, result, empty_found);
    alter_print_str("County Numeric: ", county_numeric, result, empty_found);
    alter_print_str("Primary Lat DMS: ", primary_lat_dms.str(), result, empty_found);
    alter_print_str("Primary Long DMS: ", primary_long_dms.str(), result, empty_found);
    alter_print_str("Primary Lat DEC: ", to_string(primary_lat_dec), result, empty_found);
    alter_print_str("Primary Long DEC: ", to_string(primary_long_dec), result, empty_found);
    alter_print_str("Source Lat DMS: ", source_lat_dms.str(), result, empty_found);
    alter_print_str("Source Long DMS: ", source_long_dms.str(), result, empty_found);
    alter_print_str("Source Lat DEC: ", to_string(source_lat_dec), result, empty_found);
    alter_print_str("Source Long DEC: ", to_string(source_long_dec), result, empty_found);
    alter_print_str("ELEV In M: ", to_string(elev_in_m), result, empty_found);
    alter_print_str("ELEV In Ft: ", to_string(elev_in_ft), result, empty_found);
    alter_print_str("Map Name: ", map_name, result, empty_found);
    alter_print_str("Date Created: ", date_created, result, empty_found);
    alter_print_str("Date Edited: ", date_edited, result, empty_found);
    if (empty_found){
        return "[Empty Attributes have been removed!]" + result + "\n";
    }else{
        return result + "\n";
    }
}