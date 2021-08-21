// File: utilities.h
// Purpose: Explode :)

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <sstream>

// Eearslya fix this :)
static std::vector<std::string> Split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream iss(str);

    for (std::string token; std::getline(iss, token, delimiter);) { result.push_back(std::move(token)); }

    return result;
}

#endif