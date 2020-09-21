//
// Created by Hassan S. Shavarani
//
#ifndef FINAL_PROJECT_FILEMANAGER_H
#define FINAL_PROJECT_FILEMANAGER_H

#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class FileManager {
    private:
        ofstream file_writer;
        ifstream file_reader;
        bool file_writer_is_opened;
        bool file_reader_is_opened;
        const char * f_name;
        void close_file_writer();
        void open_file_writer(bool append = false);
        void close_file_reader();
        void open_file_reader();
    public:
        ~FileManager();
        FileManager(): file_writer(nullptr), file_writer_is_opened(false), f_name(nullptr){}
        static vector<string> read_file(const string& filename);
        void create_file_to_write(const char * filename);
        void write_to_file(const string & str);
        vector<string> retrieve_records(const std::set<int>& record_offsets);
};


#endif //FINAL_PROJECT_FILEMANAGER_H
