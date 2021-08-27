// File: Double2.h
// Purpose: Used for vector math

#pragma once

#include <math.h>

#include <sstream>
#include <string>

struct Double2 {
 public:
	double x, y;

	Double2(double x, double y);
	Double2();

	double Magnitude() const;
	double MagnitudeSquared() const;
	Double2 Normalized() const;
	Double2 Absolute() const;
	Double2 Sign() const;
	Double2 Round() const;
	Double2 ClampMagnitude(double min, double max) const;
	bool IsUniform() const;

	std::string ToString() const;

	// Assignment operator overloads
	Double2& operator+=(const Double2& b);
	Double2& operator-=(const Double2& b);
	Double2& operator*=(const Double2& b);
	Double2& operator*=(const double& b);
	Double2& operator/=(const Double2& b);
	Double2& operator/=(const double& b);
};

Double2 Double2FromString(const std::string& s);
bool TryDouble2FromString(const std::string& s, Double2& result);

double Dot(const Double2& a, const Double2& b);
Double2 Reflect(const Double2& vec, const Double2& norm);
Double2 Clamp(const Double2& vec, const Double2& min, const Double2& max);
Double2 Lerp(const Double2& a, const Double2& b, double t);

// Operator overloads
Double2 operator+(const Double2& a, const Double2& b);
Double2 operator-(const Double2& a, const Double2& b);
Double2 operator*(const Double2& a, const Double2& b);
Double2 operator*(const Double2& a, const double& b);
Double2 operator/(const Double2& a, const Double2& b);
Double2 operator/(const Double2& a, const double& b);