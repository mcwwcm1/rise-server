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

	string str();
};

static float dot(Float3 a, Float3 b);
static Float3 cross(Float3 a, Float3 b);
static Float3 reflect(Float3 vec, Float3 norm);

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