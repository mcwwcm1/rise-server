// File: float4.h
// Purpose: Used for vector math

#ifndef FLOAT4_H
#define FLOAT4_H

#include <string>
#include <math.h>
#include <sstream>
#include "float3.h"

using namespace std;

struct Float4
{
	public:
		float x, y, z, w;

	Float4(float x, float y, float z, float w);
	Float4();

	float magnitude();
	float magnitudeSquared();
	Float4 normalized();
	Float4 absolute();
	Float4 sign();
	Float4 round();
	bool isUniform();

	Float3 xyz();

	string str();
};

Float4 clamp(Float4 vec, Float4 min, Float4 max);

Float4 operator + (const Float4 a, const Float4 b);
Float4 operator - (const Float4 a, const Float4 b);
Float4 operator * (const Float4 a, const Float4 b);
Float4 operator * (const Float4 a, const float b);
Float4 operator / (const Float4 a, const Float4 b);
Float4 operator / (const Float4 a, const float b);

Float4 operator += (const Float4 a, const Float4 b);
Float4 operator -= (const Float4 a, const Float4 b);
Float4 operator *= (const Float4 a, const Float4 b);
Float4 operator *= (const Float4 a, const float b);
Float4 operator /= (const Float4 a, const Float4 b);
Float4 operator /= (const Float4 a, const float b);

#endif