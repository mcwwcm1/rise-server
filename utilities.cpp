#include "utilities.h"

#include <btBulletDynamicsCommon.h>
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

std::string Vector3ToString(const btVector3& vec)
{
	std::stringstream ss;
	ss << "[" << vec.getX() << ";" << vec.getY() << ";" << vec.getZ() << "]";
	return ss.str();
}

std::string QuaternionToString(const btQuaternion& q)
{
	std::stringstream ss;
	ss << "[" << q.getX() << ";" << q.getY() << ";" << q.getZ() << ";" << q.getW() << "]";
	return ss.str();
}