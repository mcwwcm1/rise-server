// File: echo.h
// Purpose: Defines an echo function and its accompanying parser

#pragma once

#include "Core/Commands.h"
#include "Core/Send.h"

void Echo()
{
	// Store string pointer from top of argument buffer
	std::string echoString = Commands::GetArgument<std::string>();
	// Call Send function with string pointer from the argument buffer
	Send(echoString);
}

void EchoParser(const std::string& arguments)
{
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	// Insert accompanying function
	Commands::functionBuffer.Put(Echo);
	// Insert arguments (in this case just the input string)
	// Note: For strings a "new" keyword is required to ensure the string survives the scope
	//       This is only required for datatypes with undefined size
	Commands::argumentBuffer.Put(PrimaryArgument(arguments));
}