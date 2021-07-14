// File: double4.h
// Purpose: Used for vector math

#ifndef FLOAT4_H
#define FLOAT4_H

#include <string>
#include <math.h>
#include <sstream>
#include "double3.h"

using namespace std;

struct Double4
{
	public:
		double x, y, z, w;

	Double4(double x, double y, double z, double w);
	Double4();

	double magnitude();
	double magnitudeSquared();
	Double4 normalized();
	Double4 absolute();
	Double4 sign();
	Double4 round();
	bool isUniform();

	Double3 xyz();

	string str();
};

Double4 clamp(Double4 vec, Double4 min, Double4 max);

Double4 operator + (const Double4 a, const Double4 b);
Double4 operator - (const Double4 a, const Double4 b);
Double4 operator * (const Double4 a, const Double4 b);
Double4 operator * (const Double4 a, const double b);
Double4 operator / (const Double4 a, const Double4 b);
Double4 operator / (const Double4 a, const double b);

Double4 operator += (const Double4 a, const Double4 b);
Double4 operator -= (const Double4 a, const Double4 b);
Double4 operator *= (const Double4 a, const Double4 b);
Double4 operator *= (const Double4 a, const double b);
Double4 operator /= (const Double4 a, const Double4 b);
Double4 operator /= (const Double4 a, const double b);

#endif