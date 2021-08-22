#include "utilities.h"

#include <stdio.h>

std::vector<std::string> Split(const std::string& str, char delimiter)
{
	std::vector<std::string> result;
	std::istringstream iss(str);

	for (std::string token; std::getline(iss, token, delimiter);) {
		result.push_back(std::move(token));
	}

	return result;
}

void BoostFail(boost::beast::error_code ec, char const* what)
{
	printf("Boost failure: %s\n", ec.message().c_str());
}