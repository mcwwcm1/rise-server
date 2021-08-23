// File: utilities.h
// Purpose: Explode :)

#pragma once

#define _USE_MATH_DEFINES

#include <boost/beast.hpp>

#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "mysticmath/double3.h"

std::vector<std::string> Split(const std::string& str, char delimiter);
void BoostFail(boost::beast::error_code ec, char const* what);

namespace Random
{
void Seed(unsigned int seed);
unsigned int RandomUint(unsigned int min = 0,
                        unsigned int max = std::numeric_limits<unsigned int>::max() - 1);
double RandomDouble(double min = std::numeric_limits<double>::min(),
                    double max = std::numeric_limits<double>::max());
Double3 RandomDouble3(Double3 min = std::numeric_limits<double>::min(),
                      Double3 max = std::numeric_limits<double>::max());
Double3 RandomInUnitSphere();
unsigned int WeightedRandom(const std::vector<double>& weights);
}  // namespace Random