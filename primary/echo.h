// File: echo.h
// Purpose: Defines an echo function and its accompanying parser

#ifndef ECHO_H
#define ECHO_H

void Echo()
{
	// Grab string pointer from the buffer and store it
	std::string* argumentString = argumentBuffer.get().var.sval;
	// Attempt to assign the appropriate type to this god damn variable
	auto const ss = boost::make_shared<std::string const>(std::move(*argumentString));
	// Invoke the send function of the headless' websocket session
	headlessSession->send(ss);
	// Delete the old string to avoid leak
	delete argumentString;
	// Debug
	printf("Function: Echo\n");
}

void EchoParser(std::string& arguments)
{
	// Grab the lock for both buffers to safely insert parameters
	bufferAccessMutex.lock();
	// Insert accompanying function
	functionBuffer.put(Echo);
	// Insert arguments (in this case just the input string)
	// Note: For strings a "new" keyword is required to ensure the string survives the scope
	//       This is only required for datatypes with undefined size
	argumentBuffer.put(PrimaryArgument(new std::string(arguments)));
	// Unlock buffers
	bufferAccessMutex.unlock();
	// Debug
	printf("Parser: Echo\n");
}

#endif
