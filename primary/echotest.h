// File: indexes.h
// Purpose: Initializes the prearranged function array and parsing map

#ifndef INDEXES_H
#define INDEXES_H

// Just to test if this all is even possible
void EchoTest()
{
  //std::string* testString = argumentBuffer.get().var.sval;
  //int testInt = argumentBuffer.get().var.ival;
  std::string* argumentString = argumentBuffer.get().var.sval;
  auto const ss =
      boost::make_shared<std::string const>(std::move(*argumentString));
  //headlessSession->mutex_.lock();
  headlessSession->send(ss);
  //headlessSession->mutex_.unlock();
  if (*argumentString == "test") { printf("Test received!\n"); }
  delete argumentString;

  printf("Primary Function has run\n");
}

void EchoTestParser(std::string& arguments)
{
  //Parsing shit goes here

  //Lock the buffers to safely write to them
  bufferAccessMutex.lock();
  //Put function pointer
  functionBuffer.put(EchoTest);
  //Put test element on the argument buffer
  //PrimaryArgument currentArgument;
  //currentArgument.type = PrimaryArgument::is_int;
  //currentArgument.var.ival = 6;
  //currentArgument.type = PrimaryArgument::is_string;
  //currentArgument.var.sval = new std::string(arguments);
  //printf(("Argument placed: "+ *currentArgument.var.sval + "\n").c_str());
  argumentBuffer.put(PrimaryArgument(new std::string(arguments)));
  //Unlock buffers
  bufferAccessMutex.unlock();

  printf(("Echo test parsing function has run with argument string: " +
          arguments + "\n")
             .c_str());
}

#endif
