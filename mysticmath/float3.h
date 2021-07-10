// File: float3.h
// Purpose: Used for vector math

#ifndef FLOAT3_H
#define FLOAT3_H

#include <string>
#include <math.h>
#include <sstream>

using namespace std;

struct Float3
{
	public:
		float x, y, z;

	Float3(float x, float y, float z);
	Float3();

	float magnitude();
	float magnitudeSquared();
	Float3 normalized();
	Float3 absolute();
	Float3 sign();
	Float3 round();
	bool isUniform();

	string str();
};

float dot(Float3 a, Float3 b);
Float3 cross(Float3 a, Float3 b);
Float3 tripleProduct(Float3 a, Float3 b, Float3 c);
Float3 reflect(Float3 vec, Float3 norm);
Float3 clamp(Float3 vec, Float3 min, Float3 max);

Float3 operator + (const Float3 a, const Float3 b);
Float3 operator - (const Float3 a, const Float3 b);
Float3 operator * (const Float3 a, const Float3 b);
Float3 operator * (const Float3 a, const float b);
Float3 operator / (const Float3 a, const Float3 b);
Float3 operator / (const Float3 a, const float b);

Float3 operator += (const Float3 a, const Float3 b);
Float3 operator -= (const Float3 a, const Float3 b);
Float3 operator *= (const Float3 a, const Float3 b);
Float3 operator *= (const Float3 a, const float b);
Float3 operator /= (const Float3 a, const Float3 b);
Float3 operator /= (const Float3 a, const float b);

#endif