// File: double4.h
// Purpose: Used for vector math

#pragma once

#include <math.h>

#include <sstream>
#include <string>

#include "Double3.h"

struct Double4 {
 public:
	double x, y, z, w;

	Double4(double x, double y, double z, double w);
	Double4();

	double Magnitude() const;
	double MagnitudeSquared() const;
	Double4 Normalized() const;
	Double4 Absolute() const;
	Double4 Sign() const;
	Double4 Round() const;
	bool IsUniform() const;

	Double3 xyz() const;

	Double4 operator+(const Double4& b) const;
	Double4 operator-(const Double4& b) const;
	Double4 operator*(const Double4& b) const;
	Double4 operator*(const double b) const;
	Double4 operator/(const Double4& b) const;
	Double4 operator/(const double b) const;

	std::string ToString() const;
};

Double4 Clamp(Double4 vec, Double4 min, Double4 max);

Double4& operator+=(const Double4& a, const Double4& b);
Double4& operator-=(const Double4& a, const Double4& b);
Double4& operator*=(const Double4& a, const Double4& b);
Double4& operator*=(const Double4& a, const double b);
Double4& operator/=(const Double4& a, const Double4& b);
Double4& operator/=(const Double4& a, const double b);