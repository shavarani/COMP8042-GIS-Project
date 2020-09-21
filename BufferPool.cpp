//
// Created by hassan on 2020-09-17.
//

#include "BufferPool.h"


std::string BufferPool::str(){
    return "";
}

void BufferPool::create_db_file(const char* db_file_adr) {
    db_file.create_file_to_write(db_file_adr);
}

FileManager* BufferPool::get_db_file(){
    return &db_file;
}

vector<GISRecord> BufferPool::retrieve_records(const std::set<int>& record_offsets){
    vector<GISRecord> result;
    if(record_offsets.empty())
        return result;
    for (auto& elem: db_file.retrieve_records(record_offsets)) {
        GISRecord e(elem.first);
        e.set_file_offset(elem.second);
        result.insert(result.end(), e);
    }
    return result;
}