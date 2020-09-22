//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_LOGGER_H
#define FINAL_PROJECT_LOGGER_H

#include "FileManager.h"
#include <sstream>

class Logger {
    // You should begin the log with a few lines identifying yourself
        // then listing the names of the input files that are being used.
        // then echo each comment line, and each command that you process to the log file
        // 		- Each command (except for “world”) should be numbered, starting with 1
    private:
        FileManager log_file;
        int command_id = 0;
    public:
        Logger( const Logger & rhs ) = delete; // Copy Constructor
        Logger( Logger && rhs ) = delete; // Move Constructor
        Logger & operator= ( const Logger & rhs ) = delete;
        Logger & operator= ( Logger && rhs ) = delete;
        explicit Logger(const char* log_file_adr = nullptr);
        ~Logger() = default;

        void log_command(const string & command);
        void log_file_names(const char* db_file_name, const char* script_file_name, const char* log_file_name);
        void log_command_output(const string& log);
        void log_time(const string & time_name = "Run Time:");
};


#endif //FINAL_PROJECT_LOGGER_H
