// File: globals.h
// Purpose: Intializes global variables

//Declare the map to be used for command parsing (populated in main)
std::unordered_map<std::string, parsing_function> parseMap;

//Define struct for containing function arguments
struct PrimaryArgument {
	enum {is_int, is_float, is_double, is_string} type;
	union {
		int ival;
		float fval;
		double dval;
		std::string* sval;
	} var;
	//Simple constructors
	PrimaryArgument() {}
	PrimaryArgument(int i) {
		var.ival = i;
		type = is_int;
	}
	PrimaryArgument(float f) {
		var.fval = f;
		type = is_float;
	}
	PrimaryArgument(double d) {
		var.dval = d;
		type = is_double;
	}
	PrimaryArgument(std::string* s) {
		var.sval = s;
		type = is_string;
	}
};

//Set up global mutex for managing buffer access
std::mutex bufferAccessMutex;

//Scopeless declaration of buffers
CircularBuffer<primary_function> functionBuffer(1000);
CircularBuffer<PrimaryArgument> argumentBuffer(2000);

//Forward declaration
class session;
//Mutex to protect headlessSession pointer
std::mutex headlessPointerMutex;
//Pointer to session object of headless user
session* headlessSession = NULL;
std::unordered_map<std::string, session*> registeredUsers;
