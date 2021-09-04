// File: echo.h
// Purpose: Defines an echo function that sends to a specific user and its accompanying parser

#pragma once

#include "Core/Commands.h"
#include "Core/Send.h"
#include "Utilities.h"

void EchoTo()
{
	// Store string pointers from top of argument buffer
	std::string userIDString = Commands::GetArgument<std::string>();
	std::string message      = Commands::GetArgument<std::string>();
	// Call Send function with string pointer from the argument buffer
	Send(userIDString, message);
}

void EchoToParser(const std::string& arguments)
{
	std::lock_guard<std::mutex> lock(Commands::bufferAccessMutex);

	auto data = Split(arguments, ' ');

	// Insert accompanying function
	Commands::functionBuffer.Put(EchoTo);
	// Insert arguments (in this case just the input string)
	// Note: For strings a "new" keyword is required to ensure the string survives the scope
	//       This is only required for datatypes with undefined size
	Commands::argumentBuffer.Put(PrimaryArgument(data[0]));
	Commands::argumentBuffer.Put(PrimaryArgument(data[1]));
}