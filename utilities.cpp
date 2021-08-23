#include "utilities.h"

#include <cassert>
#include <cmath>
#include <random>
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

namespace Random
{
static std::random_device gRandomDevice;
static std::mt19937 gMersenne(gRandomDevice());

void Seed(unsigned int seed)
{
	gMersenne.seed(seed);
}

unsigned int RandomUint(unsigned int min, unsigned int max)
{
	assert(max > min);

	std::uniform_int_distribution<unsigned int> gen(min, max);

	return gen(gMersenne);
}

double RandomDouble(double min, double max)
{
	assert(max > min);

	std::uniform_real_distribution<double> gen(min, max);

	return gen(gMersenne);
}

Double3 RandomDouble3(Double3 min, Double3 max)
{
	assert(max.x > min.x && max.y > min.y && max.z > min.z);

	std::uniform_real_distribution<double> x(min.x, max.x);
	std::uniform_real_distribution<double> y(min.y, max.y);
	std::uniform_real_distribution<double> z(min.z, max.z);

	return Double3(x(gMersenne), y(gMersenne), z(gMersenne));
}

Double3 RandomInUnitSphere()
{
	double u        = RandomDouble();
	double v        = RandomDouble();
	double theta    = u * 2.0 * M_PI;
	double phi      = acos(2.0 * v - 1.0);
	double r        = cbrt(RandomDouble());
	double sinTheta = sin(theta);
	double cosTheta = cos(theta);
	double sinPhi   = sin(phi);
	double cosPhi   = cos(phi);
	double x        = r * sinPhi * cosTheta;
	double y        = r * sinPhi * sinTheta;
	double z        = r * cosPhi;

	return Double3(x, y, z);
}

unsigned int WeightedRandom(const std::vector<double>& weights)
{
	std::discrete_distribution weightedRandom(weights.begin(), weights.end());

	return weightedRandom(gMersenne);
}
}  // namespace Random