// File: double3.cpp
// Purpose: Implements Double3

#include "double3.h"

#include <algorithm>

#include "mysticmath.h"

Double3::Double3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Double3::Double3(double value) : Double3(value, value, value) {}

Double3::Double3()
{
	x = 0;
	y = 0;
	z = 0;
}

double Double3::MagnitudeSquared() const { return x * x + y * y + z * z; }

double Double3::Magnitude() const
{
	float squared = MagnitudeSquared();
	return squared == 0 ? 0 : sqrt(squared);
}

Double3 Double3::Normalized() const
{
	double m = Magnitude();
	return m == 0 ? Double3() : Double3(x / m, y / m, z / m);
}

Double3 Double3::Absolute() const { return *this * this->Sign(); }

Double3 Double3::Sign() const
{
	return Double3(x > 0, y > 0, z > 0) - Double3(x < 0, y < 0, z < 0);
}

Double3 Double3::Round() const
{
	return Double3(std::round(x), std::round(y), std::round(z));
}

bool Double3::IsUniform() const { return x == y && y == z; }

std::string Double3::ToString() const
{
	std::stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << "]";
	return ss.str();
}

Double3 Double3FromString(const std::string& s)
{
	size_t sep1 = s.find(';');
	size_t sep2 = s.find(';', sep1 + 1);
	double x    = std::stof(s.substr(s.find('[') + 1, sep1 - 1));
	double y    = std::stof(s.substr(sep1 + 1, sep2 - sep1));
	double z    = std::stof(s.substr(sep2 + 1, s.length() - sep2 - 1));
	return Double3(x, y, z);
}

bool TryDouble3FromString(const std::string& s, Double3& result)
{
	try {
		result = Double3FromString(s);
		return true;
	} catch (const std::exception& e) {
		return false;
	}
}

double Dot(const Double3& a, const Double3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Double3 Cross(const Double3& a, const Double3& b)
{
	return Double3(
			a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Double3 TripleProduct(const Double3& a, const Double3& b, const Double3& c)
{
	return Cross(a, Cross(b, c));
}

Double3 Reflect(const Double3& vec, const Double3& norm)
{
	return vec - (norm * 2 * Dot(vec, norm));
}

Double3 Clamp(const Double3& vec, const Double3& min, const Double3& max)
{
	return Double3(std::min(std::max(vec.x, min.x), max.x),
	               std::min(std::max(vec.y, min.y), max.y),
	               std::min(std::max(vec.z, min.z), max.z));
}

Double3 Lerp(const Double3& a, const Double3& b, double t)
{
	return a * (1 - t) + b * t;
}

// Assignment operator overloads
Double3::operator btVector3()
{
	return btVector3(x, y, z);
}

Double3& Double3::operator=(const btVector3& b)
{
	this->x = b.getX();
	this->y = b.getY();
	this->z = b.getZ();
	return *this;
}

Double3& Double3::operator+=(const Double3& b)
{
	*this = *this + b;
	return *this;
}

Double3& Double3::operator-=(const Double3& b)
{
	*this = *this - b;
	return *this;
}

Double3& Double3::operator*=(const Double3& b)
{
	*this = *this * b;
	return *this;
}

Double3& Double3::operator*=(const double& b)
{
	*this = *this * b;
	return *this;
}

Double3& Double3::operator/=(const Double3& b)
{
	*this = *this / b;
	return *this;
}

Double3& Double3::operator/=(const double& b)
{
	*this = *this / b;
	return *this;
}

// Operator overloads
Double3 operator+(const Double3& a, const Double3& b)
{
	return Double3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Double3 operator-(const Double3& a, const Double3& b)
{
	return Double3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Double3 operator*(const Double3& a, const Double3& b)
{
	return Double3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Double3 operator*(const Double3& a, const double& b)
{
	return Double3(a.x * b, a.y * b, a.z * b);
}

Double3 operator/(const Double3& a, const Double3& b)
{
	return Double3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Double3 operator/(const Double3& a, const double& b)
{
	return Double3(a.x / b, a.y / b, a.z / b);
}