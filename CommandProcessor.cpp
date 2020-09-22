//
// Created by Hassan S. Shavarani
//

#include "CommandProcessor.h"

enum Command {
    WORLD=0, WHAT_IS=1, WHAT_IS_AT=2, WHAT_IS_IN=3, QUIT=4, IMPORT=5, DEBUG=6
};

Command get_command(const string& command_name){
    if (command_name == "world"){
        return WORLD;
    } else if (command_name == "quit"){
        return QUIT;
    } else if (command_name == "import"){
        return IMPORT;
    } else if (command_name == "debug"){
        return DEBUG;
    } else if (command_name == "what_is"){
        return WHAT_IS;
    } else if (command_name == "what_is_at"){
        return WHAT_IS_AT;
    } else if (command_name == "what_is_in"){
        return WHAT_IS_IN;
    } else {
        throw std::invalid_argument("Invalid command name <"+command_name+">!");
    }
}


CommandProcessor::CommandProcessor(const char* db_file, const char* script_file, const char* log_file):
        logger(log_file), systemManager(db_file) {
    this -> script_lines = FileManager::read_file(script_file);
    cout << "Reading the script file in CommandProcessor..." << endl;
    logger.log_file_names(db_file, script_file, log_file);
    logger.log_time("Start Time:");
    for (const auto& command: this->script_lines) {
        logger.log_command(command);
        if (command.empty() || command.find(";") == 0){
            continue;
        }
        vector<string> command_parts;
        split(command, command_parts, '\t');
        process_command(command_parts);
    }
}

void CommandProcessor::process_command(const vector<string>& arguments){
    auto itr = arguments.begin();
    const string command = *itr++;
    switch(get_command(command)) {
        case WORLD: {
            string west = *itr++;
            string east = *itr++;
            string north = *itr++;
            string south = *itr++;
            logger.log_command_output(systemManager.process_world_command(west, east, north, south));
            if (itr != arguments.end())
                throw std::invalid_argument("World command only receives 4 arguments");
            break;
        }
        case IMPORT:
            logger.log_command_output(systemManager.process_import_command(*itr++));
            if (itr != arguments.end())
                throw std::invalid_argument("Import command only receives 1 argument");
            break;
        case WHAT_IS: {
            string feature_name = *itr++;
            string state_abbreviation = *itr++;
            logger.log_command_output(systemManager.process_what_is_command(feature_name, state_abbreviation));
            if (itr != arguments.end())
                throw std::invalid_argument("What_is command only receives 2 arguments");
            break;
        }
        case WHAT_IS_AT: {
            string geographic_coordinate_lat = *itr++;
            string geographic_coordinate_long = *itr++;
            logger.log_command_output(
                    systemManager.process_what_is_at_command(geographic_coordinate_lat, geographic_coordinate_long));
            if (itr != arguments.end())
                throw std::invalid_argument("What_is_at command only receives 2 arguments");
            break;
        }
        case WHAT_IS_IN: {
            string filter;
            bool long_report = false;
            string first = *itr++;
            string second = *itr++;
            if (first == "-long") {
                long_report = true;
                first = second;
                second = *itr++;
            } else if (first == "-filter") {
                filter = second;
                first = *itr++;
                second = *itr++;
            }
            int half_height = stoi(*itr++);
            int half_width = stoi(*itr++);
            logger.log_command_output(
                    systemManager.process_what_is_in_command(first, second, half_height, half_width, filter,
                                                             long_report));
            while(itr != arguments.end() && (*itr++).empty())
                continue;
            if (itr != arguments.end())
                throw std::invalid_argument("What_is_in command receives at most 5 arguments");
            break;
        }
        case QUIT:
            logger.log_command_output(systemManager.process_quit_command());
            logger.log_time("End time:");
            break;
        case DEBUG:
            logger.log_command_output(systemManager.process_debug_command(*itr++));
            if (itr != arguments.end())
                throw std::invalid_argument("Debug command only receives 1 argument");
            break;
        default:
            throw std::invalid_argument("Invalid command " + command + " provided!");
    }
}