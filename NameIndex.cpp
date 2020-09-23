//
// Created by Hassan S. Shavarani
//

#include "NameIndex.h"


void NameIndex::index_record(const std::string& feature_name, const std::string& state_alpha, int record_offset){
    string key = feature_name + ":" + state_alpha;
    NameIndexElement* elem = index.search(key);
    if (elem == nullptr) {
        elem = new NameIndexElement(key);
        if(!index.insert(key, *elem))
            throw std::invalid_argument("Expansion is necessary here!");
    }
    elem = index.search(key);
    elem->insert(record_offset);
    all_name_length += feature_name.length();
    index_size++;
}

std::set<int> NameIndex::lookup_record(const std::string& feature_name, const std::string& state_alpha){
    string key = feature_name + ":" + state_alpha;
    NameIndexElement* elem = index.search(key);
    if (elem == nullptr)
        return std::set<int>{};
    else
        return elem->get_record_offsets();
}

int NameIndex::get_index_size() const {
    return index_size;
}

int NameIndex::get_longest_probe_sequence() const {
    return index.getLongestProbe();
}

int NameIndex::get_average_name_length() const {
    if (index_size > 0)
        return all_name_length / index_size;
    else
        return 0;
}

std::string NameIndex::str() {
    std::ostringstream os;
    os  << "Format of display is " << std::endl << "Slot number: data record" << std::endl
        << "Current table size is " << index.getTableSize() << std::endl << "Number of elements in table is " << index_size
        << std::endl << std::endl;
    os << index.str();
    return os.str();
}