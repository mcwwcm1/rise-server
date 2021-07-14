// File: fail.h
// Purpose: Implements a fail method

#ifndef FAIL_H
#define FAIL_H

#include <unordered_map>

//Declare the map to be used for command parsing (populated in main)
std::unordered_map<std::string, parsing_function> parseMap;

//Failure Reporting
void fail(boost::beast::error_code ec, char const* what)
{
	std::cerr << what << ": " << ec.message() << "\n";
}

#endif
