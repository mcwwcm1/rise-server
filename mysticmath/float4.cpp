// File: float4.h
// Purpose: Implements Float4

#include "float4.h"
#include "mysticmath.h"
#include <algorithm>

Float4::Float4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Float4::Float4()
{
	x = y = z = w = 0;
}

float Float4::magnitudeSquared()
{
	return x * x + y * y + z * z;
}

float Float4::magnitude()
{
	return sqrt(magnitudeSquared());
}

Float4 Float4::normalized()
{
	float m = magnitude();
	return Float4(x / m, y / m, z / m, w / m);
}

Float4 Float4::absolute()
{
	return *this * this->sign();
}

Float4 Float4::sign()
{
	return Float4(x > 0, y > 0, z > 0, w > 0) - Float4(x < 0, y < 0, z < 0, w < 0);
}

Float4 Float4::round()
{
	return Float4(roundf(x), roundf(y), roundf(z), roundf(w));
}

bool Float4::isUniform()
{
	return x == y && y == z && z == w;
}

Float3 Float4::xyz()
{
	return Float3(x, y, z);
}

string Float4::str()
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << ";" << w << "]";
	return ss.str();
}

float dot(Float4 a, Float4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Float4 clamp(Float4 vec, Float4 min, Float4 max)
{
	return Float4(
		std::min(std::max(vec.x, min.x), max.x), 
		std::min(std::max(vec.y, min.y), max.y), 
		std::min(std::max(vec.z, min.z), max.z),
		std::min(std::max(vec.w, min.w), max.w));
}

Float4 operator + (Float4 a,Float4 b)
{
	return Float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Float4 operator - (Float4 a, Float4 b)
{
	return Float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Float4 operator * (Float4 a, Float4 b)
{
	return Float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Float4 operator * (Float4 a, float b)
{
	return Float4(a.x * b, a.y * b, a.z * b, a.w * b);
}

Float4 operator / (Float4 a, Float4 b)
{
	return Float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

Float4 operator / (Float4 a, float b)
{
	return Float4(a.x / b, a.y / b, a.z / b, a.w / b);
}