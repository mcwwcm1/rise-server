// File: double3.cpp
// Purpose: Implements Double3

#include "double3.h"
#include "mysticmath.h"
#include <algorithm>

Double3::Double3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Double3::Double3()
{
	x = y = z = 0;
}

double Double3::magnitudeSquared() const
{
	return x * x + y * y + z * z;
}

double Double3::magnitude() const
{
	return sqrt(magnitudeSquared());
}

Double3 Double3::normalized() const
{
	double m = magnitude();
	return Double3(x / m, y / m, z / m);
}

Double3 Double3::absolute() const
{
	return *this * this->sign();
}

Double3 Double3::sign() const
{
	return Double3(x > 0, y > 0, z > 0) - Double3(x < 0, y < 0, z < 0);
}

Double3 Double3::round() const
{
	return Double3(std::round(x), std::round(y), std::round(z));
}

bool Double3::isUniform() const
{
	return x == y && y == z;
}

string Double3::str() const
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << "]";
	return ss.str();
}

double dot(const Double3& a, const Double3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Double3 cross(const Double3& a, const Double3& b)
{
	return Double3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Double3 tripleProduct(const Double3& a, const Double3& b, const Double3& c)
{
	return cross(a, cross(b, c));
}

Double3 clamp(const Double3& vec, const Double3& min, const Double3& max)
{
	return Double3(std::min(std::max(vec.x, min.x), max.x), std::min(std::max(vec.y, min.y), max.y), std::min(std::max(vec.z, min.z), max.z));
}

// Assignment operator overloads
Double3& Double3::operator += (const Double3& b)
{
	*this = *this + b;
	return *this;
}

Double3& Double3::operator -= (const Double3& b)
{
	*this = *this - b;
	return *this;
}

Double3& Double3::operator *= (const Double3& b)
{
	*this = *this * b;
	return *this;
}

Double3& Double3::operator *= (const double& b)
{
	*this = *this * b;
	return *this;
}

Double3& Double3::operator /= (const Double3& b)
{
	*this = *this / b;
	return *this;
}

Double3& Double3::operator /= (const double& b)
{
	*this = *this / b;
	return *this;
}

// Operator overloads
Double3 operator + (const Double3& a, const Double3& b)
{
	return Double3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Double3 operator - (const Double3& a, const Double3& b)
{
	return Double3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Double3 operator * (const Double3& a, const Double3& b)
{
	return Double3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Double3 operator * (const Double3& a, const double& b)
{
	return Double3(a.x * b, a.y * b, a.z * b);
}

Double3 operator / (const Double3& a, const Double3& b)
{
	return Double3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Double3 operator / (const Double3& a, const double& b)
{
	return Double3(a.x / b, a.y / b, a.z / b);
}