// File: indexes.h
// Purpose: Initializes the prearranged function array and parsing map

#ifndef INDEXES_H
#define INDEXES_H

//Declare a test type for function pointer
typedef void (*primary_function) ();
typedef void (*parsing_function) (std::string&);

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

// Just to test if this all is even possible
void EchoTest()
{
	//See Parser for explanation
	bufferAccessMutex.lock();
	double testDouble = argumentBuffer.get().var.dval;
	bufferAccessMutex.unlock();

	printf("Primary Function has run\n");
}

void EchoTestParser(std::string& arguments)
{
	//Parsing shit goes here

	//Lock the buffers to safely write to them
	bufferAccessMutex.lock();
	//Put function pointer
	functionBuffer.put(EchoTest);
	//Unlock buffers
	bufferAccessMutex.unlock();

	printf("Parsing Function has run\n");
}


#endif
