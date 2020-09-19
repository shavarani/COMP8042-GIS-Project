//
// Created by hassan on 2020-09-17.
//

#ifndef FINAL_PROJECT_NAMEINDEX_H
#define FINAL_PROJECT_NAMEINDEX_H

#include <map>
#include <set>

class NameIndex {
    // will use a hash table for its physical organization.

        // Each hash table entry will store
        //		1. a feature name and state abbreviation (separately or concatenated, as you like)
        //		2. the file offset(s) of the matching record(s).

        // Hash Table Implementation:
        // 		Your table will use quadratic probing to resolve collisions, with the quadratic function ((n^2+n)/2)to compute the step size.
        // 		The hash table must use a contiguous physical structure (array).
        // 		The initial size of the table will be 1024
        // 		The table will resize itself automatically, by doubling its size whenever the table becomes 70% full.

        // You must be able to display the contents of the hash table in a readable manner.
    private:
        int table_size = 1024;
        double load_factor = 0.0;
        // TODO implement a hashmap here!
        std::map<unsigned long, std::set<int>> index;
        int index_size = 0.0;
        int longest_probe_sequence = 0.0;
        int all_name_length = 0.0;

public:
    ~NameIndex() = default;
    NameIndex() : index() {}
    NameIndex( const NameIndex & rhs ) = default; // Copy Constructor
    NameIndex( NameIndex && rhs ) = default; // Move Constructor
    NameIndex & operator= ( const NameIndex & rhs ) = default; // Copy Assignment
    NameIndex & operator= ( NameIndex && rhs ) = default; // Move Assignment

    void index_record(const std::string& feature_name, const std::string& state_alpha, int record_offset);
    int get_index_size() const;
    int get_longest_probe_sequence() const;
    int get_average_name_length() const;
    std::string str();
};


#endif //FINAL_PROJECT_NAMEINDEX_H
