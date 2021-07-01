// File: float3.h
// Purpose: Implements Float3

#include "float3.h"
#include "mysticmath.h"

Float3::Float3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Float3::Float3()
{
	x = y = z = 0;
}

float Float3::magnitudeSquared()
{
	return x * x + y * y + z * z;
}

float Float3::magnitude()
{
	return sqrt(magnitudeSquared());
}

Float3 Float3::normalized()
{
	float m = magnitude();
	return Float3(x / m, y / m, z / m);
}

Float3 Float3::absolute()
{
	return *this * this->sign();
}

Float3 Float3::sign()
{
	return Float3(x > 0, y > 0, z > 0) - Float3(x < 0, y < 0, z < 0);
}

string Float3::str()
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << "]";
	return ss.str();
}

float dot(Float3 a, Float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Float3 cross(Float3 a, Float3 b)
{
	return Float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Float3 operator + (Float3 a,Float3 b)
{
	return Float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Float3 operator - (Float3 a, Float3 b)
{
	return Float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Float3 operator * (Float3 a, Float3 b)
{
	return Float3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Float3 operator * (Float3 a, float b)
{
	return Float3(a.x * b, a.y * b, a.z * b);
}

Float3 operator / (Float3 a, Float3 b)
{
	return Float3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Float3 operator / (Float3 a, float b)
{
	return Float3(a.x / b, a.y / b, a.z / b);
}