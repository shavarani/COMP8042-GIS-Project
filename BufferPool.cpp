//
// Created by hassan on 2020-09-17.
//

#include "BufferPool.h"


std::string BufferPool::str(){
    return "";
}

void BufferPool::create_db_file(const char* db_file_adr) {
    db_file.create_file(db_file_adr);
}

FileManager* BufferPool::get_db_file(){
    return &db_file;
}

BufferPool::~BufferPool() {
    db_file.close_file();
}