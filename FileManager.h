//
// Created by Hassan S. Shavarani
//
#ifndef FINAL_PROJECT_FILEMANAGER_H
#define FINAL_PROJECT_FILEMANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class FileManager {
    private:
        ofstream file;
        bool file_is_opened;
    public:
        ~FileManager();
        FileManager(): file(nullptr), file_is_opened(false){}
        static vector<string> read_file(const string& filename);
        void create_file(const char * filename);
        void write_to_file(const string & str);
};


#endif //FINAL_PROJECT_FILEMANAGER_H
