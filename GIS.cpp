#include "CommandProcessor.h"


int main(int argc, char const *argv[]) {
	if (argc != 4){
		string command_format = "You need to pass all input arguments to the program:\n";
		command_format += "\t\t\t./GIS <database file name> <command script file name> <log file name>";
		throw std::invalid_argument(command_format);
	}
    CommandProcessor cp(argv[1], argv[2], argv[3]);
	return 0;
}