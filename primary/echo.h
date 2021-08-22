// File: echo.h
// Purpose: Defines an echo function and its accompanying parser

#ifndef ECHO_H
#define ECHO_H

void Echo()
{
	// Store string pointer from top of argument buffer
	std::string* echoString = argumentBuffer.Get().var.sval;
	// Call Send function with string pointer from the argument buffer
	Send(echoString);
	// Delete the old string to avoid leak
	delete echoString;
	// Debug
	printf("Function: Echo\n");
}

void EchoParser(std::string& arguments)
{
	// Grab the lock for both buffers to safely insert parameters
	bufferAccessMutex.lock();
	// Insert accompanying function
	functionBuffer.Put(Echo);
	// Insert arguments (in this case just the input string)
	// Note: For strings a "new" keyword is required to ensure the string survives the scope
	//       This is only required for datatypes with undefined size
	argumentBuffer.Put(PrimaryArgument(new std::string(arguments)));
	// Unlock buffers
	bufferAccessMutex.unlock();
	// Debug
	printf("Parser: Echo\n");
}

#endif
