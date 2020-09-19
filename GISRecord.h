//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_GISRECORD_H
#define FINAL_PROJECT_GISRECORD_H

#include <string>
#include <limits>
#include <vector>
#include "DMS.cpp"

using namespace std;

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ') {
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

class GISRecord {
    /*
        Since each GIS record occupies one line in the file, it is a trivial matter to locate and read a record
        given nothing but the file offset at which the record begins.
        */

        /*
        Attributes:
            1. GIS record file will contain two or more distinct records that have the same geographic coordinates.
            2. coordinates are expressed in the usual DMS system (they cannot be primary/unique key).
            3. name index : records will be indexed by the Feature Name and State (abbreviation) fields.
            4. coordinate index: indexed by geographic coordinate.

        */
    private:
        //Each index should have the ability to write a nicely-formatted display of itself to an output stream.
        // ???
        std::string name_index;
        std::string coordinate_index;
        int feature_id{};
        std::string feature_name;
        std::string feature_class;
        std::string state_alpha;
        std::string state_numeric;
        std::string county_name;
        std::string county_numeric;
        DMS primary_lat_dms;
        DMS primary_long_dms;
        double primary_lat_dec;
        double primary_long_dec;
        DMS source_lat_dms;
        DMS source_long_dms;
        double source_lat_dec;
        double source_long_dec;
        int elev_in_m{};
        int elev_in_ft{};
        std::string map_name;
        std::string date_created;
        std::string date_edited;

    public:
        GISRecord( const GISRecord & rhs ) = default; // Copy Constructor
        GISRecord( GISRecord && rhs ) = default; // Move Constructor
        GISRecord & operator= ( const GISRecord & rhs ) = default; // Copy Assignment
        GISRecord & operator= ( GISRecord && rhs ) = default; // Move Assignment
        explicit GISRecord(const std::string & raw_record);
        ~GISRecord() = default;  // This is the destructor declaration

        DMS get_primary_lat_dms() const;
        DMS get_primary_long_dms() const;
        std::string get_feature_name() const;
        std::string get_state_alpha() const;
        GISRecord retrieve_record(const std::string& criteria);
        std::string str();
};


#endif //FINAL_PROJECT_GISRECORD_H
