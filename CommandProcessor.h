//
// Created by Hassan S. Shavarani
//

#ifndef FINAL_PROJECT_COMMANDPROCESSOR_H
#define FINAL_PROJECT_COMMANDPROCESSOR_H

#include <string>
#include <vector>
#include "SystemManager.h"
#include "Logger.h"

class CommandProcessor {
    // Duties:
        //		1. manages retrieving commands from the script file
        // 		2. makes the necessary calls to other components in order to carry out those commands.
    private:
        Logger logger;
        std::vector<std::string> script_lines;
        SystemManager systemManager;
    public:
        CommandProcessor(const char *db_file, const char *script_file, const char *log_file);
        CommandProcessor( const CommandProcessor & rhs ) = delete; // Copy Constructor
        CommandProcessor( CommandProcessor && rhs ) = delete; // Move Constructor
        CommandProcessor & operator= ( const CommandProcessor & rhs ) = delete; // Copy Assignment
        CommandProcessor & operator= ( CommandProcessor && rhs ) = delete; // Move Assignment
        bool process_command(const std::vector<std::string>& arguments);
};


#endif //FINAL_PROJECT_COMMANDPROCESSOR_H
