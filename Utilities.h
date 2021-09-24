// File: utilities.h
// Purpose: Explode :)

#pragma once

#define _USE_MATH_DEFINES

#include <boost/beast.hpp>

#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Core/Commands.h"
#include "MysticMath/Double3.h"

std::vector<std::string> Split(const std::string& str, char delimiter);
std::string VectorToString(const std::vector<std::string> vec, char delimiter, bool trailingDelimiter = true);
void BoostFail(boost::beast::error_code ec, char const* what);

namespace Random
{
void Seed(unsigned int seed);
unsigned int RandomUint(unsigned int min = 0, unsigned int max = std::numeric_limits<unsigned int>::max() - 1);
double RandomDouble(double min = std::numeric_limits<double>::min(), double max = std::numeric_limits<double>::max());
Double3 RandomDouble3(Double3 min = std::numeric_limits<double>::min(), Double3 max = std::numeric_limits<double>::max());
Double3 RandomInUnitSphere();
Double3 RandomUnitVector();
unsigned int WeightedRandom(const std::vector<double>& weights);
}  // namespace Random

class Parser
{
 public:
	Parser(const std::string& str, std::optional<uint32_t> requiredArgumentCount = std::nullopt)
			: _parts(Commands::Explode(str, '|'))
	{
		if (requiredArgumentCount.has_value() && _parts.size() != requiredArgumentCount.value()) {
			throw std::invalid_argument("Expected " + std::to_string(requiredArgumentCount.value()) + " arguments, only received " +
			                            std::to_string(_parts.size()));
		}
	}

	template <typename T>
	T Get()
	{
		std::istringstream iss(_parts[_index++]);
		T value;
		iss >> value;

		return value;
	}

 private:
	std::vector<std::string> _parts;
	uint32_t _index = 0;
};

template <>
inline bool Parser::Get<bool>()
{
	return _parts[_index++] == "True";
}