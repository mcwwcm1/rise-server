// File: quaternion.h
// Purpose: For working with quaternions

#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>

#include <sstream>
#include <string>

#include "double3.h"
#include "mysticmath.h"

using namespace std;

struct Quaternion {
 public:
	double x, y, z, w;
	static const Quaternion identity;  // Unsure on this

	Quaternion(double x, double y, double z, double w);
	Quaternion();

	double Magnitude() const;
	double MagnitudeSquared() const;
	Quaternion Conjugate() const;
	Quaternion Normalized() const;
	Double3 ToEuler() const;

	static Quaternion FromEuler(Double3 euler);

	string ToString() const;

	Quaternion& operator+=(const Quaternion& b);
	Quaternion& operator-=(const Quaternion& b);
	Quaternion& operator*=(const Quaternion& b);
	Quaternion& operator*=(double b);
};

Quaternion QuaternionFromString(string s);
bool TryQuaternionFromString(string s, Quaternion& result);

Quaternion FromToRotation(const Quaternion& a, const Quaternion& b);
Quaternion FromToRotation(const Double3& a, const Double3& b);

Quaternion operator+(const Quaternion& a, const Quaternion& b);
Quaternion operator-(const Quaternion& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, const Quaternion& b);
Double3 operator*(const Double3& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, double b);

#endif