// File: globals.h
// Purpose: Intializes global variables

//Declare the map to be used for command parsing (populated in main)
std::unordered_map<std::string, parsing_function> parseMap;

//Define struct for containing function arguments
struct PrimaryArgument {
	enum {is_int, is_float, is_double} type;
	union {
		int ival;
		float fval;
		double dval;
	} var;
};

//Set up global mutex for managing buffer access
std::mutex bufferAccessMutex;

//Scopeless declaration of buffers
CircularBuffer<primary_function> functionBuffer(1000);
CircularBuffer<PrimaryArgument> argumentBuffer(1000);

//Forward declaration
class session;
//Pointer to session object of headless user
session* headlessSession = NULL;
