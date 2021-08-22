// File: utilities.h
// Purpose: Explode :)

#pragma once

#include <boost/beast.hpp>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> Split(const std::string& str, char delimiter);
void BoostFail(boost::beast::error_code ec, char const* what);