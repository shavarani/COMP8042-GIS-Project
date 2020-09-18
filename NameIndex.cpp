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

void NameIndex::index_record(std::string feature_name, std::string state_alpha, int record_offset){
    unsigned long key = elf_hash(feature_name, state_alpha);
    if(!index.count(key))
        index.insert(std::pair<unsigned long,std::set<int>>(key, {}));
    index[key].insert(record_offset);
    index_size++;
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
