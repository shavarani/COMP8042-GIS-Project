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
        throw std::invalid_argument("Unable to open the file \"" + filename + "\" , it might not exist, or there are access permission problems, exiting ...");
    return data;
}

void FileManager::create_file_to_write(const char * filename){
    remove(filename);
    /*if (remove(filename) == 0){
        cout << "Previously created file <" << filename <<"> successfully re-created ..." << endl;
    }*/
    f_name = filename;
    open_file_writer();
}

FileManager::~FileManager(){
    close_file_writer();
}

void FileManager::write_to_file(const string & str) {
    open_file_writer(true);
    file_writer << str;
}

void FileManager::open_file_writer(bool append){
    if(!append && file_writer_is_opened)
        file_writer.close();
    if(!file_writer_is_opened){
        if (f_name == nullptr)
            throw std::invalid_argument("You need to initialize the file manager first!");
        if (append)
            file_writer.open(f_name, std::ios_base::app);
        else
            file_writer.open(f_name);
        file_writer_is_opened = true;
    }
}

void FileManager::close_file_writer(){
    if(file_writer_is_opened){
        file_writer.close();
        file_writer_is_opened = false;
    }
}

void FileManager::close_file_reader(){
    if(file_reader_is_opened){
        file_reader.close();
        file_reader_is_opened = false;
    }
}

void FileManager::open_file_reader(){
    if (f_name == nullptr)
        throw std::invalid_argument("You need to initialize the file manager first!");
    if(!file_reader_is_opened){
        file_reader.open(f_name);
        file_reader_is_opened = true;
    }
}

vector<std::pair <std::string,int>> FileManager::retrieve_records(const std::set<int>& record_offsets){
    vector<std::pair <std::string,int>> res;
    if (record_offsets.empty())
        return res;
    close_file_writer();
    open_file_reader();
    int line_counter = 0;
    if(file_reader.is_open()) {
        std::string line;
        //file_reader.seekg(0, ios::end);
        while(!file_reader.eof()) {
            getline(file_reader,line);
            if (record_offsets.count(line_counter))
                res.insert(res.end(), std::make_pair(line, line_counter));
            line_counter++;
            if (record_offsets.size() == res.size())
                break;
        }
    }
    close_file_reader();
    return res;
}