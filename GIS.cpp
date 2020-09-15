#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <string>
#include <ctime>
#include <limits>
#include <sstream>

using namespace std;

vector<string> read_file(const string& filename){
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

ofstream create_file(const char * filename){
    if (remove(filename) == 0){
        cout << "Previously created file <" << filename <<"> successfully re-created ..." << endl;
    }
    ofstream file;
    file.open(filename);
    return file;
}

// The string split function
template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ') {
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

class GISRecord {
	/*
	Since each GIS record occupies one line in the file, it is a trivial matter to locate and read a record 
	given nothing but the file offset at which the record begins.
	*/
    private:
    int feature_id;
    string feature_name;
    string feature_class;
    string state_alpha;
    string state_numeric;
    string county_name;
    string county_numeric;
    string primary_lat_dms;
    string primary_long_dms;
    double primary_lat_dec;
    double primary_long_dec;
    string source_lat_dms;
    string source_long_dms;
    double source_lat_dec;
    double source_long_dec;
    int elev_in_m;
    int elev_in_ft;
    string map_name;
    string date_created;
    string date_edited;

    static void alter_print_str(const string & name, const string & value, string & result,
                                bool & empty_found, const string & delim = " --- "){
        if (!value.empty()){
            result += delim + name + value;
        } else{
            empty_found = true;
        }
    }

    public:
        explicit GISRecord(const string & raw_record){
            if(raw_record.empty()){
                return;
            }
            vector<string> gis_record_tokens;
            split(raw_record, gis_record_tokens, '|');
            auto itr = gis_record_tokens.begin();
            feature_id = std::stoi(*itr++);
            feature_name = *itr++;
            feature_class = *itr++;
            state_alpha = *itr++;
            state_numeric = *itr++;
            county_name = *itr++;
            county_numeric = *itr++;
            primary_lat_dms = *itr++;
            primary_long_dms = *itr++;
            string plad = *itr++;
            string plod = *itr++;
            //Note: Records showing "Unknown" and zeros for the latitude and longitude DMS and decimal fields,
            //respectively, indicate that the coordinates of the feature are unknown. They are recorded in the
            //database as zeros to satisfy the format requirements of a numerical data type. They are not errors and
            //do not reference the actual geographic coordinates at 0 latitude, 0 longitude.
            primary_lat_dec = plad.empty()? 0.0 : std::stof(plad);
            primary_long_dec = plod.empty()? 0.0 : std::stof(plod);
            source_lat_dms = *itr++;
            source_long_dms = *itr++;
            string slad = *itr++;
            string slod = *itr++;
            source_lat_dec = slad.empty()? 0.0 : std::stof(slad);
            source_long_dec = slod.empty()? 0.0 : std::stof(slod);
            string eim = *itr++;
            string eif = *itr++;
            elev_in_m = eim.empty()? -std::numeric_limits<int>::max(): std::stoi(eim);
            elev_in_ft = eif.empty()? -std::numeric_limits<int>::max(): std::stoi(eif);
            map_name = *itr++;
            date_created = *itr++;
            date_edited = *itr++;
            //print();
		}
		~GISRecord() = default;  // This is the destructor declaration
		GISRecord retrieve_record(const string& criteria) {
            return static_cast<GISRecord>(nullptr);
        }

        string print(){
		    bool empty_found = false;
		    string result;
            alter_print_str("Feature Id: ", to_string(feature_id), result, empty_found);
            alter_print_str("Feature Name: ", feature_name, result, empty_found);
            alter_print_str("Feature Class: ", feature_class, result, empty_found);
            alter_print_str("State Alpha: ", state_alpha, result, empty_found);
            alter_print_str("State Numeric: ", state_numeric, result, empty_found);
            alter_print_str("County Name: ", county_name, result, empty_found);
            alter_print_str("County Numeric: ", county_numeric, result, empty_found);
            alter_print_str("Primary Lat DMS: ", primary_lat_dms, result, empty_found);
            alter_print_str("Primary Long DMS: ", primary_long_dms, result, empty_found);
            alter_print_str("Primary Lat DEC: ", to_string(primary_lat_dec), result, empty_found);
            alter_print_str("Primary Long DEC: ", to_string(primary_long_dec), result, empty_found);
            alter_print_str("Source Lat DMS: ", source_lat_dms, result, empty_found);
            alter_print_str("Source Long DMS: ", source_long_dms, result, empty_found);
            alter_print_str("Source Lat DEC: ", to_string(source_lat_dec), result, empty_found);
            alter_print_str("Source Long DEC: ", to_string(source_long_dec), result, empty_found);
            alter_print_str("ELEV In M: ", to_string(elev_in_m), result, empty_found);
            alter_print_str("ELEV In Ft: ", to_string(elev_in_ft), result, empty_found);
            alter_print_str("Map Name: ", map_name, result, empty_found);
            alter_print_str("Date Created: ", date_created, result, empty_found);
            alter_print_str("Date Edited: ", date_edited, result, empty_found);
            if (empty_found){
                return "[Empty Attributes have been removed!]" + result + "\n";
            }else{
                return result + "\n";
            }
		}

    //Each index should have the ability to write a nicely-formatted display of itself to an output stream.
		string name_index;
		string coordinate_index;
	/*
	Attributes:
		1. GIS record file will contain two or more distinct records that have the same geographic coordinates.
		2. coordinates are expressed in the usual DMS system (they cannot be primary/unique key).
		3. name index : records will be indexed by the Feature Name and State (abbreviation) fields.
		4. coordinate index: indexed by geographic coordinate.

	*/
};

class NameIndex {
	// will use a hash table for its physical organization.

	// Each hash table entry will store 
	//		1. a feature name and state abbreviation (separately or concatenated, as you like) 
	//		2. the file offset(s) of the matching record(s).

	// Hash Table Implementation:
	// 		Your table will use quadratic probing to resolve collisions, with the quadratic function ((n^2+n)/2)to compute the step size.
	// 		The hash table must use a contiguous physical structure (array). 
	// 		The initial size of the table will be 1024
	// 		The table will resize itself automatically, by doubling its size whenever the table becomes 70% full.

	// You must be able to display the contents of the hash table in a readable manner.
};

class CoordinateIndex {
	// Will use a bucket PR quadtree for the physical organization.
	//		in which each leaf stores up to K data objects (here K=4)
	// The PR quadtree implementation should follow good design practices, 
	//      and its interface should be somewhat similar to that of the BST.

	// The index entries held in the quadtree will store 
	//		1. a geographic coordinate
	// 		2. a collection of the file offsets of the matching GIS records in the database file.

	// Quadtree implementation notes:
	// You are expected to implement different types for the leaf and internal nodes, 
	//		with appropriate data membership for each, 
	//	and an abstract base type from which they are both derived.

	// You must be able to display the PR quadtree in a readable manner.
	//		The display must clearly indicate the structure of the tree, 
	//		the relationships between its nodes, and the data objects in the leaf nodes.

	//Quadtree children are printed in the order SW  SE  NE  NW
};

class BufferPool {
	/* 
		Will be used when performing search
		Will be bypassed when performing import
	*/
	// Should be capable of buffering up to 15 records (will use LRU replacement)
	// You may use any structure you like to organize the pool slots
	// It can both store GIS record objects or just strings

	// You must be able to display the contents of the buffer pool, listed from MRU to LRU entry, in a readable manner.
	public:
		BufferPool() = default;
		~BufferPool() = default;
};

class Rectangle {
	// a class containing a spatial region information	
};

class Logger {
    // You should begin the log with a few lines identifying yourself
    // then listing the names of the input files that are being used.
    // then echo each comment line, and each command that you process to the log file
    // 		- Each command (except for “world”) should be numbered, starting with 1
    private:
        ofstream log_file;
        int command_id = 0;
    public:
        explicit Logger(const char* log_file_adr = nullptr): log_file(nullptr){
            this -> log_file = create_file(log_file_adr);
            this -> log_file << "GIS Program log:" << endl;
        }

        ~Logger(){
            this -> log_file.close();
        }

        void log_command(const string & command){
            if (command.empty() || command.find(";") == 0 || command.substr(0, 5) == "world"){
                this -> log_file << command << endl;
            } else{
                this -> log_file << "Command " << ++command_id << ": "<<command << endl;
            }
        }

        void log_file_names(const char* db_file_name, const char* script_file_name, const char* log_file_name){
            this -> log_file << "dbFile:\t" << db_file_name << endl;
            this -> log_file << "script:\t" << script_file_name << endl;
            this -> log_file << "log:\t" << log_file_name << endl;
        }

        void log_printable_log(const string& log){
            this->log_file << log << endl;
        }

        void log_time(const string & time_name = "Run Time:"){
            string monthString[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
            string dayString[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            time_t now = time(nullptr);
            tm *ltm = localtime(&now);
            this->log_file << time_name << " " << dayString[ltm->tm_wday] << " " << monthString[ltm->tm_mon] << " " <<
                    ltm->tm_mday << " " << (ltm->tm_hour < 10 ? "0":"") << ltm->tm_hour << ":" <<
                    (ltm->tm_min < 10 ? "0":"") << ltm->tm_min << ":" << (ltm->tm_sec < 10 ? "0":"") <<
                    ltm->tm_sec << " " << ltm->tm_zone << " " << 1900 + ltm->tm_year << endl;
        }
};

enum Command { WORLD=0, WHAT_IS=1, WHAT_IS_AT=2, WHAT_IS_IN=3, QUIT=4, IMPORT=5, DEBUG=6 };

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


class World {
    private:
        string west_long_dms;
        string east_long_dms;
        string south_lat_dms;
        string north_lat_dms;
        double west_long_dec;
        double east_long_dec;
        double south_lat_dec;
        double north_lat_dec;
    public:
        World(const string& west_long, const string& east_long,
              const string& south_lat, const string& north_lat):
                  west_long_dms(west_long), east_long_dms(east_long),
                  south_lat_dms(south_lat), north_lat_dms(north_lat){
            //			- features that lie outside the specified coordinate space simply will not be indexed.
            /*
                Quadtree children are printed in the order SW  SE  NE  NW
                --------------------------------------------------------------------------------

                Latitude/longitude values in index entries are shown as signed integers, in total seconds.

                World boundaries are set to:
                              138600
                   -396000                -367200
                              109800
                --------------------------------------------------------------------------------
             * */
            west_long_dec = convert_dms_to_dec(west_long);
            east_long_dec = convert_dms_to_dec(east_long);
            south_lat_dec = convert_dms_to_dec(south_lat);
            north_lat_dec = convert_dms_to_dec(north_lat);
        }

        string print() {
            string log_tabs = "\t\t\t\t\t\t";
            std::ostringstream os;
            os  << "------------------------------------------------------------------------------------------" << endl
                << "Latitude/longitude values in index entries are shown as signed integers, in total seconds." <<endl
                << "------------------------------------------------------------------------------------------" << endl
                <<  log_tabs << "World boundaries are set to:"<< endl
                <<  log_tabs << "           " << (north_lat_dec < 0? "-": "") << north_lat_dms.substr(0, 6) << endl
                <<  log_tabs << (west_long_dec < 0? "-": "") << west_long_dms.substr(0, 6) << "              " << (east_long_dec < 0? "-": "") << east_long_dms.substr(0, 6) << endl
                <<  log_tabs << "           " << (south_lat_dec < 0? "-": "") << south_lat_dms.substr(0, 6) << endl
                << "------------------------------------------------------------------------------------------" << endl;
            return os.str();
        }

        double static convert_dms_to_dec(const string& dms){
            if (dms.length()!= 7 && dms.length() != 8){
                throw std::invalid_argument("DMS values are expected to be of sizes 7 or 8");
            }
            bool neg(false) ;
            bool is_latitude(false);
            if (dms[dms.length() - 1] == 'W'){
                neg = true;
            } else if (dms[dms.length() - 1] == 'S') {
                is_latitude = true;
                neg = true;
            } else if (dms[dms.length() - 1] == 'N') {
                is_latitude = true;
            } else if (dms[0] == '-') {
                throw std::invalid_argument("DMS values are expected to show the sign using the direction modifiers at the end");
            }
            double deg, min_, sec;
            deg = stof(dms.substr(0, is_latitude? 2: 3));
            min_= stof(dms.substr(is_latitude? 2: 3, 2));
            sec = stof(dms.substr(is_latitude? 4: 5, 2));
            double ang = deg + ((min_ + (sec / 60.0)) / 60.0) ;
            return neg? -ang: ang;
        }
};

class SystemManager {
    // Use cases:
    // 		1. validates the command line arguments
    // 		2. manages the initialization of the various system components.
private:
    ofstream db_file;
    World* world;
public:
    ~SystemManager() = default;
    //SystemManager( const SystemManager & rhs ) = default; // Copy Constructor
    //SystemManager( SystemManager && rhs ) = default; // Move Constructor
    //SystemManager & operator= ( const SystemManager & rhs )= default; // Copy Assignment
    //SystemManager & operator= ( SystemManager && rhs )= default; // Move Assignment

    explicit SystemManager(const char* db_file_adr = nullptr): db_file(nullptr), world(nullptr){
        this -> db_file = create_file(db_file_adr);
        this -> db_file << "DB_FILE:" << endl;
    }

    void process_debug_command(const string & component_name){
        //		5. Debug command:
        //			- debug<tab>[ quad | hash | pool | world]
        //			- Log the contents of the specified index structure
        //				- include key values and file offsets
        if (component_name == "world") {
            cout << "debugging " + component_name << endl;
        } else if (component_name == "quad") {
            cout << "debugging " + component_name << endl;
        } else if (component_name == "hash") {
            cout << "debugging " + component_name << endl;
        } else if (component_name == "pool") {
            cout << "debugging " + component_name << endl;
        } else {
            throw std::invalid_argument("Invalid component_name: "+component_name);
        }
    }

    void process_quit_command(){
        //		6. Quit Command
        //			- quit<tab>
        //			- Terminate program execution.
        cout << "quitting ..." << endl;
    }

    string process_world_command(const string& west_long, const string& east_long,
                               const string& south_lat, const string& north_lat){
        // 		3. World command (occur once):
        //			- The first non-comment line will specify the world boundaries (in DMS format) to be used:
        //			- world<tab><westLong><tab><eastLong><tab><southLat><tab><northLat>
        World wd(west_long, east_long, south_lat, north_lat);
        world = &wd;
        return wd.print();
    }

    void process_import_command(const string & gis_record_file_name){
        //		4. Import command:
        //			- import<tab><GIS record file>
        //			- Add all the valid GIS records in the specified file to the database file.
        //				- records will be appended to the existing database file
        //				- those records will be indexed in the manner described earlier.
        //			- When completed, log
        //				- the number of entries added to each index,
        //				- the longest probe sequence that was needed when inserting to the hash table.
        vector<string> gis_records = read_file("../GIS_FILES/"+gis_record_file_name);
        //GISRecord record(gis_records[10]);
        bool first_record_seen = false;
        for (const auto& gis_record: gis_records) {
            if (first_record_seen)
                GISRecord record(gis_record);
            first_record_seen = true;
            //delete &record;
        }
        // Done properly!
    }

    // TODO		-- optional -sort flag for "what is" commands needs to be worked out!
    void process_what_is_command(const string & feature_name, const string & state_abbreviation){
        //		7. what is command
        //			- what is<tab><feature name><tab><state abbreviation>
        //			- find every GIS record in the database file that matches the given <feature name> and <state abbreviation>
        //			- for each record log only
        //				- the offset (at which the record was found)
        //				- the county name
        //				- the primary latitude
        //				- the primary longitude
        cout << "args: " << feature_name << ", " << state_abbreviation << endl;
    }

    // TODO		-- optional -sort flag for "what is at" commands needs to be worked out!
    void process_what_is_at_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long){
        //		8. what is at command
        //			- what is at<tab><geographic coordinate>
        //			- find every GIS record in the database file that matches the given <geographic coordinate>,
        //				- for each record log only log:
        //					- the offset (at which the record was found)
        //					- the feature name
        //					- county name
        //					- state abbreviation
        cout << "args: " << geographic_coordinate_lat << ", " << geographic_coordinate_long << endl;
    }

    // TODO		-- optional -sort flag for "what is in" commands needs to be worked out!
    void process_what_is_in_command(const string & geographic_coordinate_lat, const string & geographic_coordinate_long,
                                    const string & half_height, const string & half_width, const string & filter, bool long_report){
        //		9. What is in command
        //			- what is in<tab><geographic coordinate><tab><half-height><tab><half-width>
        //				- optional -long : display of a long listing of the relevant records
        //									- The switch will be the first token following the name of the command.
        //				- optional -filter [ pop | water | structure ]
        //									- The switch and its modifier will be the first and second tokens following the name of the command.
        //			- find every GIS record in the database file whose coordinates fall within the closed rectangle
        //					(with the specified height and width [specified as seconds], centered at the <geographic coordinate>)
        //				- if [-filter] provided: filter the set of matching records to only show those whose feature type field corresponds to the given filter specifier (Table 2 in the Appendix).
        //					- if [-long] provided: for each record log every important non-empty field, nicely formatted and labeled.
        //					- else: for each record only log
        //						- the offset (at which the record was found)
        //						- the feature name
        //						- the state name
        //						- the primary latitude
        //						- the primary longitude
        cout << "args: " << geographic_coordinate_lat << ", " << geographic_coordinate_long << ", +/-" << half_height << ", +/-" << half_width << ", filter: " << filter << ", long? "<< (long_report?"true":"false") << endl;
    }
};

class CommandProcessor {
    // Duties:
    //		1. manages retrieving commands from the script file
    // 		2. makes the necessary calls to other components in order to carry out those commands.
    private:
        Logger logger;
        vector<string> script_lines;
        SystemManager systemManager;
    public:
        explicit CommandProcessor(const char* db_file, const char* script_file, const char* log_file):
                logger(log_file), systemManager(db_file) {
            this -> script_lines = read_file(script_file);
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
        void process_command(const vector<string>& arguments){
            auto itr = arguments.begin();
            const string command = *itr++;
            switch(get_command(command)) {
                case WORLD: {
                    string west = *itr++;
                    string east = *itr++;
                    string north = *itr++;
                    string south = *itr++;
                    logger.log_printable_log(systemManager.process_world_command(west, east, north, south));
                    if (itr != arguments.end())
                        throw std::invalid_argument("World command only receives 4 arguments");
                    break;
                }
                case IMPORT:
                    systemManager.process_import_command(*itr++);
                    if (itr != arguments.end())
                        throw std::invalid_argument("Import command only receives 1 argument");
                    break;
                case WHAT_IS:
                    systemManager.process_what_is_command(*itr++, *itr++);
                    if (itr != arguments.end())
                        throw std::invalid_argument("What_is command only receives 2 arguments");
                    break;
                case WHAT_IS_AT:
                    systemManager.process_what_is_at_command(*itr++, *itr++);
                    if (itr != arguments.end())
                        throw std::invalid_argument("What_is_at command only receives 2 arguments");
                    break;
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
                    systemManager.process_what_is_in_command(first, second, *itr++, *itr++, filter, long_report);
                    while(itr != arguments.end() && (*itr++).empty())
                        continue;
                    if (itr != arguments.end())
                        throw std::invalid_argument("What_is_in command receives at most 5 arguments");
                    break;
                }
                case QUIT:
                    systemManager.process_quit_command();
                    logger.log_time("End time:");
                    break;
                case DEBUG:
                    systemManager.process_debug_command(*itr++);
                    if (itr != arguments.end())
                        throw std::invalid_argument("Debug command only receives 1 argument");
                    break;
                default:
                    throw std::invalid_argument("Invalid command " + command + " provided!");
            }
            //if (itr != arguments.end())
            //    cout << "Arguments: ";
            //while(itr != arguments.end()){
            //    cout << *itr++ << endl;
            //}
            //for(const auto& command_part: command_parts)
            //    cout << command_part << endl;

        }
};







int main(int argc, char const *argv[]) {
	if (argc != 4){
		string command_format = "You need to pass all input arguments to the program:\n";
		command_format += "\t\t\t./GIS <database file name> <command script file name> <log file name>";
		throw std::invalid_argument(command_format);
	}
	//ofstream db_file = create_file();
    CommandProcessor cp(argv[1], argv[2], argv[3]);



	/*    stuff    */
	//db_file << "Please write this text to a file.\n this text is written using C++\n";
	//log_file << "Test data";
	//db_file.close();
	//log_file.close();
	return 0;
}