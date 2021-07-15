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

	double magnitude() const;
	double magnitudeSquared() const;
	Double4 normalized() const;
	Double4 absolute() const;
	Double4 sign() const;
	Double4 round() const;
	bool isUniform() const;

	Double3 xyz();

	Double4 operator + (const Double4& b);
	Double4 operator - (const Double4& b);
	Double4 operator * (const Double4& b);
	Double4 operator * (const double b);
	Double4 operator / (const Double4& b);
	Double4 operator / (const double b);

	string str();
};

Double4 clamp(Double4 vec, Double4 min, Double4 max);

Double4& operator += (const Double4& a, const Double4& b);
Double4& operator -= (const Double4& a, const Double4& b);
Double4& operator *= (const Double4& a, const Double4& b);
Double4& operator *= (const Double4& a, const double b);
Double4& operator /= (const Double4& a, const Double4& b);
Double4& operator /= (const Double4& a, const double b);

#endif