// File: double3.h
// Purpose: Used for vector math

#ifndef DOUBLE3_H
#define DOUBLE3_H

#include <math.h>

#include <sstream>
#include <string>

using namespace std;

struct Double3 {
 public:
	double x, y, z;

	Double3(double x, double y, double z);
	Double3();

	double Magnitude() const;
	double MagnitudeSquared() const;
	Double3 Normalized() const;
	Double3 Absolute() const;
	Double3 Sign() const;
	Double3 Round() const;
	Double3 ClampMagnitude(double min, double max) const;
	bool IsUniform() const;

	string ToString() const;

	// Assignment operator overloads
	Double3& operator+=(const Double3& b);
	Double3& operator-=(const Double3& b);
	Double3& operator*=(const Double3& b);
	Double3& operator*=(const double& b);
	Double3& operator/=(const Double3& b);
	Double3& operator/=(const double& b);
};

Double3 Double3FromString(string s);
bool TryDouble3FromString(string s, Double3& result);

double Dot(const Double3& a, const Double3& b);
Double3 Cross(const Double3& a, const Double3& b);
Double3 TripleProduct(const Double3& a, const Double3& b, const Double3& c);
Double3 Reflect(const Double3& vec, const Double3& norm);
Double3 Clamp(const Double3& vec, const Double3& min, const Double3& max);
Double3 Lerp(const Double3& a, const Double3& b, double t);

// Operator overloads
Double3 operator+(const Double3& a, const Double3& b);
Double3 operator-(const Double3& a, const Double3& b);
Double3 operator*(const Double3& a, const Double3& b);
Double3 operator*(const Double3& a, const double& b);
Double3 operator/(const Double3& a, const Double3& b);
Double3 operator/(const Double3& a, const double& b);

#endif