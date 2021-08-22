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

	double magnitude() const;
	double magnitudeSquared() const;
	Double3 normalized() const;
	Double3 absolute() const;
	Double3 sign() const;
	Double3 round() const;
	Double3 clampMagnitude(double min, double max) const;
	bool isUniform() const;

	string str() const;

	// Assignment operator overloads
	Double3& operator+=(const Double3& b);
	Double3& operator-=(const Double3& b);
	Double3& operator*=(const Double3& b);
	Double3& operator*=(const double& b);
	Double3& operator/=(const Double3& b);
	Double3& operator/=(const double& b);
};

Double3 double3FromString(string s);
bool tryDouble3FromString(string s, Double3& result);

double dot(const Double3& a, const Double3& b);
Double3 cross(const Double3& a, const Double3& b);
Double3 tripleProduct(const Double3& a, const Double3& b, const Double3& c);
Double3 reflect(const Double3& vec, const Double3& norm);
Double3 clamp(const Double3& vec, const Double3& min, const Double3& max);
Double3 lerp(const Double3& a, const Double3& b, double t);

// Operator overloads
Double3 operator+(const Double3& a, const Double3& b);
Double3 operator-(const Double3& a, const Double3& b);
Double3 operator*(const Double3& a, const Double3& b);
Double3 operator*(const Double3& a, const double& b);
Double3 operator/(const Double3& a, const Double3& b);
Double3 operator/(const Double3& a, const double& b);

#endif