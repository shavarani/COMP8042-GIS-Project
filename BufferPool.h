//
// Created by hassan on 2020-09-17.
//

#ifndef FINAL_PROJECT_BUFFERPOOL_H
#define FINAL_PROJECT_BUFFERPOOL_H

#include <string>
#include <vector>
#include <set>
#include "GISRecord.h"
#include "FileManager.h"

class BufferPool {
    /*
        Will be used when performing search
        Will be bypassed when performing import
    */
    // Should be capable of buffering up to 15 records (will use LRU replacement)
    // You may use any structure you like to organize the pool slots
    // It can both store GIS record objects or just strings

    // You must be able to display the contents of the buffer pool, listed from MRU to LRU entry, in a readable manner.
    private:
        vector<GISRecord> pool;
        FileManager db_file;
    public:
        BufferPool(): db_file(){}
        BufferPool( const BufferPool & rhs ) = delete; // Copy Constructor
        BufferPool( BufferPool && rhs ) = delete; // Move Constructor
        BufferPool & operator= ( const BufferPool & rhs ) = delete; // Copy Assignment
        BufferPool & operator= ( BufferPool && rhs ) = delete; // Move Assignment
        ~BufferPool() = default;

        std::string str();
        void create_db_file(const char* db_file_adr);
        FileManager* get_db_file();
        vector<GISRecord> retrieve_records(const std::set<int>& record_offsets);
};


#endif //FINAL_PROJECT_BUFFERPOOL_H
