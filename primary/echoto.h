// File: echo.h
// Purpose: Defines an echo function that sends to a specific user and its accompanying parser

#ifndef ECHOTO_H
#define ECHOTO_H

void EchoTo()
{
  // Store string pointers from top of argument buffer
  std::string* userIDString = argumentBuffer.get().var.sval;
  std::string* message      = argumentBuffer.get().var.sval;
  // Call Send function with string pointer from the argument buffer
  Send(userIDString, message);
  // Delete the old string to avoid leak
  delete userIDString;
  delete message;
  // Debug
  printf("Function: EchoTo\n");
}

void EchoToParser(std::string& arguments)
{
  // Grab the lock for both buffers to safely insert parameters
  bufferAccessMutex.lock();
  // Insert accompanying function
  functionBuffer.put(EchoTo);
  // Insert arguments (in this case just the input string)
  // Note: For strings a "new" keyword is required to ensure the string survives the scope
  //       This is only required for datatypes with undefined size
  argumentBuffer.put(PrimaryArgument(
      new std::string(arguments.substr(0, arguments.find(" ")))));
  argumentBuffer.put(PrimaryArgument(new std::string(
      arguments.substr(arguments.find(" ") + 1, arguments.length()))));
  // Unlock buffers
  bufferAccessMutex.unlock();
  // Debug
  printf("Parser: EchoTo\n");
}

#endif
