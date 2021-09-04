#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <variant>
#include <vector>

#include "Core/CircularBuffer.h"
#include "MysticMath/Double3.h"
#include "MysticMath/Quaternion.h"

// Declare a test type for function pointer
typedef void (*PrimaryFunction)();
typedef void (*ParsingFunction)(const std::string&);

// Define struct for containing function arguments
class PrimaryArgument
{
 public:
	PrimaryArgument() = default;

	template <typename T>
	PrimaryArgument(const T& val)
	{
		mValue = val;
	}

	template <typename T>
	T Get()
	{
		return std::get<T>(mValue);
	}

 private:
	std::variant<int, float, double, size_t, long, std::string, Double3, Quaternion> mValue;
};

namespace Commands
{
void Register(const std::string& commandString, ParsingFunction parser);
ParsingFunction Get(const std::string& commandString);
std::vector<std::string> Explode(const std::string& str, char delimiter);

// Set up global mutex for managing buffer access
extern std::mutex bufferAccessMutex;
extern CircularBuffer<PrimaryFunction> functionBuffer;
extern CircularBuffer<PrimaryArgument> argumentBuffer;

template <typename T>
T GetArgument()
{
	return argumentBuffer.Get().Get<T>();
}

void ValidateArgumentCount(const std::vector<std::string>& args,
                           size_t expected);
}  // namespace Commands