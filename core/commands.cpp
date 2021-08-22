#include "commands.h"

#include <sstream>
#include <unordered_map>

namespace Commands
{
static std::unordered_map<std::string, ParsingFunction> parseMap;
std::mutex bufferAccessMutex;
CircularBuffer<PrimaryFunction> functionBuffer(1000);
CircularBuffer<PrimaryArgument> argumentBuffer(2000);

void Register(const std::string& commandString, ParsingFunction parser)
{
	parseMap[commandString] = parser;
}

ParsingFunction Get(const std::string& commandString)
{
	const auto cmdIt = parseMap.find(commandString);
	if (cmdIt != parseMap.end()) { return cmdIt->second; }
	return nullptr;
}

std::vector<std::string> Explode(const std::string& str, char delimiter)
{
	std::vector<std::string> result;
	std::istringstream iss(str);

	for (std::string token; std::getline(iss, token, delimiter);) {
		result.push_back(std::move(token));
	}

	return result;
}

void ValidateArgumentCount(const std::vector<std::string>& args,
                           size_t expected)
{
	if (args.size() != expected) {
		throw std::invalid_argument("Expected " + std::to_string(expected) +
		                            " arguments, only received " +
		                            std::to_string(args.size()));
	}
}
}  // namespace Commands