//
// Created by Hassan S. Shavarani
//
#include "FileManager.h"

vector<string> FileManager::read_file(const string& filename){
    vector<string> data;
    string line;
    ifstream my_file (filename);
    if(my_file.is_open()) {
        while(!my_file.eof()){
            getline (my_file,line);
            data.push_back(line);
        }
        my_file.close();
    }
    else
        throw std::invalid_argument("Unable to open file, exiting ...");
    return data;
}

void FileManager::create_file(const char * filename){
    if (remove(filename) == 0){
        cout << "Previously created file <" << filename <<"> successfully re-created ..." << endl;
    }
    if(file_is_opened)
        file.close();
    file.open(filename);
    file_is_opened = true;
}

FileManager::~FileManager(){
    if(file_is_opened){
        file.close();
        file_is_opened = false;
    }
}

void FileManager::write_to_file(const string & str) {
    if(!file_is_opened)
        throw std::invalid_argument("You need to initialize the file manager first!");
    file << str;
}