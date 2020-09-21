//
// Created by hassan on 2020-09-17.
//

#include "NameIndex.h"


unsigned long elf_hash (const std::string & feature_name, const std::string & state_alpha) {
    if (state_alpha.empty() && feature_name.empty())
        return 0;
    const char *s = (feature_name + state_alpha).c_str();
    unsigned long h = 0, high;
    while (*s) {
        h = ( h << 4 ) + *s++;
        if (high = h & 0xF0000000)
            h ^= high >> 24;
        h &= ~high;
    }
    return h;
}

void NameIndex::index_record(const std::string& feature_name, const std::string& state_alpha, int record_offset){
    unsigned long key = elf_hash(feature_name, state_alpha);
    if(!index.count(key)) {
        NameIndexElement e(feature_name + ":" + state_alpha);
        index.insert(std::pair<unsigned long, NameIndexElement>(key, e));
    }
    index[key].insert(record_offset);
    all_name_length += feature_name.length();
    index_size++;
}

std::set<int> NameIndex::lookup_record(const std::string& feature_name, const std::string& state_alpha){
    unsigned long key = elf_hash(feature_name, state_alpha);
    if(!index.count(key))
        return std::set<int>{};
    else
        return index[key].get_record_offsets();
}

int NameIndex::get_index_size() const {
    return index_size;
}

int NameIndex::get_longest_probe_sequence() const {
    return longest_probe_sequence;
}

int NameIndex::get_average_name_length() const {
    if (index_size > 0)
        return all_name_length / index_size;
    else
        return 0;
}

std::string NameIndex::str() const{
    std::ostringstream os;
    os  << "Format of display is " << std::endl << "Slot number: data record" << std::endl
        << "Current table size is " << table_size << std::endl << "Number of elements in table is " << index_size
        << std::endl << std::endl;
    auto it = index.begin();
    while (it != index.end()) {
        unsigned long key = it->first;
        NameIndexElement elem = it->second;
        os << "\t" << key << ": " << elem.str() << std::endl;
        it++;
    }
    return os.str();
}