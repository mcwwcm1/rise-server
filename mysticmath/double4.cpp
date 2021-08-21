// File: double4.h
// Purpose: Implements Double4

#include "double4.h"
#include "mysticmath.h"
#include <algorithm>

Double4::Double4(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Double4::Double4()
{
	x = y = z = w = 0;
}

double Double4::magnitudeSquared() const
{
	return x * x + y * y + z * z;
}

double Double4::magnitude() const
{
	return sqrt(magnitudeSquared());
}

Double4 Double4::normalized() const
{
	double m = magnitude();
	return Double4(x / m, y / m, z / m, w / m);
}

Double4 Double4::absolute() const
{
	return *this * sign();
}

Double4 Double4::sign() const
{
	return Double4(x > 0, y > 0, z > 0, w > 0) - Double4(x < 0, y < 0, z < 0, w < 0);
}

Double4 Double4::round() const
{
	return Double4(roundf(x), roundf(y), roundf(z), roundf(w));
}

bool Double4::isUniform() const
{
	return x == y && y == z && z == w;
}

Double3 Double4::xyz() const
{
	return Double3(x, y, z);
}

string Double4::str() const
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << ";" << w << "]";
	return ss.str();
}

double dot(const Double4 &a, const Double4 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Double4 clamp(const Double4 &vec, const Double4 &min, const Double4 &max)
{
	return Double4(
			std::min(std::max(vec.x, min.x), max.x),
			std::min(std::max(vec.y, min.y), max.y),
			std::min(std::max(vec.z, min.z), max.z),
			std::min(std::max(vec.w, min.w), max.w));
}

Double4 Double4::operator+(const Double4 &b) const
{
	return Double4(x + b.x, y + b.y, z + b.z, w + w);
}

Double4 Double4::operator-(const Double4 &b) const
{
	return Double4(x - b.x, y - b.y, z - b.z, w - b.w);
}

Double4 Double4::operator*(const Double4 &b) const
{
	return Double4(x * b.x, y * b.y, z * b.z, w * b.w);
}

Double4 Double4::operator*(double b) const
{
	return Double4(x * b, y * b, z * b, w * b);
}

Double4 Double4::operator/(const Double4 &b) const
{
	return Double4(x / b.x, y / b.y, z / b.z, w / b.w);
}

Double4 Double4::operator/(const double b) const
{
	return Double4(x / b, y / b, z / b, w / b);
}