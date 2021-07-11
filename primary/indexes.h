// File: indexes.h
// Purpose: Initializes the prearranged function array and parsing map

#ifndef INDEXES_H
#define INDEXES_H

#include <unordered_map>

//Define struct for containing function arguments
struct PrimaryArgument {
	enum {is_int, is_float, is_double} type;
	union {
		int ival;
		float fval;
		double dval;
	} var;
};

// Just to test if this all is even possible
void EchoTest(CircularBuffer<PrimaryArgument>& argumentBuffer)
{
	std::cout << "Test";
}

//Declare a test type for function pointer
typedef void (*parsing_function) (std::string&);
typedef void (*primary_function) (CircularBuffer<PrimaryArgument>&);

//Declare parsing map to be populated with strings and associate parsing functions
std::unordered_map<std::string, parsing_function> parseMap;
//Declare array of ints to be associated with executing functions in the main thread
primary_function funcArr[1];

//Populate parseMap
parseMap["echo"] = EchoTest;

//Populate funcArr

#endif
