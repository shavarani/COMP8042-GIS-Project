//
// Created by Hassan S. Shavarani
//

#include "Logger.h"

Logger::Logger(const char* log_file_adr): log_file(){
    log_file.create_file_to_write(log_file_adr);
    std::ostringstream os;
    os << "GIS Program log:" << endl;
    string res = os.str();
    log_file.write_to_file(res);
}

void Logger::log_command(const string & command){
    std::ostringstream os;
    if (command.empty() || command.find(";") == 0 || command.substr(0, 5) == "world"){
        os << command << endl;
    } else{
        os << "Command " << ++command_id << ": "<<command << endl << endl;
    }
    string res = os.str();
    log_file.write_to_file(res);
}

void Logger::log_file_names(const char* db_file_name, const char* script_file_name, const char* log_file_name){
    std::ostringstream os;
    os << "dbFile:\t" << db_file_name << endl;
    os << "script:\t" << script_file_name << endl;
    os << "log:\t" << log_file_name << endl;
    string res = os.str();
    log_file.write_to_file(res);
}

void Logger::log_printable_log(const string& log){
    std::ostringstream os;
    os << log << "------------------------------------------------------------------------------------------" << endl;
    string res = os.str();
    log_file.write_to_file(res);
}

void Logger::log_time(const string & time_name){
    string monthString[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    string dayString[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    std::ostringstream os;
    os << time_name << " " << dayString[ltm->tm_wday] << " " << monthString[ltm->tm_mon]
                   << " " << ltm->tm_mday << " " << (ltm->tm_hour < 10 ? "0":"") << ltm->tm_hour << ":" <<
                   (ltm->tm_min < 10 ? "0":"") << ltm->tm_min << ":" << (ltm->tm_sec < 10 ? "0":"") <<
                   ltm->tm_sec << " " << ltm->tm_zone << " " << 1900 + ltm->tm_year << endl;
    string res = os.str();
    log_file.write_to_file(res);
}