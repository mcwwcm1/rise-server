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

double Double4::magnitudeSquared()
{
	return x * x + y * y + z * z;
}

double Double4::magnitude()
{
	return sqrt(magnitudeSquared());
}

Double4 Double4::normalized()
{
	double m = magnitude();
	return Double4(x / m, y / m, z / m, w / m);
}

Double4 Double4::absolute()
{
	return *this * this->sign();
}

Double4 Double4::sign()
{
	return Double4(x > 0, y > 0, z > 0, w > 0) - Double4(x < 0, y < 0, z < 0, w < 0);
}

Double4 Double4::round()
{
	return Double4(roundf(x), roundf(y), roundf(z), roundf(w));
}

bool Double4::isUniform()
{
	return x == y && y == z && z == w;
}

Double3 Double4::xyz()
{
	return Double3(x, y, z);
}

string Double4::str()
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << ";" << w << "]";
	return ss.str();
}

double dot(Double4 a, Double4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Double4 clamp(Double4 vec, Double4 min, Double4 max)
{
	return Double4(
		std::min(std::max(vec.x, min.x), max.x), 
		std::min(std::max(vec.y, min.y), max.y), 
		std::min(std::max(vec.z, min.z), max.z),
		std::min(std::max(vec.w, min.w), max.w));
}

Double4 operator + (Double4 a,Double4 b)
{
	return Double4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Double4 operator - (Double4 a, Double4 b)
{
	return Double4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Double4 operator * (Double4 a, Double4 b)
{
	return Double4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Double4 operator * (Double4 a, double b)
{
	return Double4(a.x * b, a.y * b, a.z * b, a.w * b);
}

Double4 operator / (Double4 a, Double4 b)
{
	return Double4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

Double4 operator / (Double4 a, double b)
{
	return Double4(a.x / b, a.y / b, a.z / b, a.w / b);
}