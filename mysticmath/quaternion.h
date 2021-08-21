// File: quaternion.h
// Purpose: For working with quaternions

#ifndef QUATERNION_H
#define QUATERNION_H

#include <string>
#include <sstream>
#include <math.h>
#include "double3.h"
#include "mysticmath.h"

using namespace std;

struct Quaternion
{
	public:
		double x, y, z, w;
		static const Quaternion identity; // Unsure on this


	Quaternion(double x, double y, double z, double w);
	Quaternion();
	
	double magnitude() const;
	double magnitudeSquared() const;
	Quaternion conjugate() const;
	Quaternion normalized() const;
	Double3 toEuler() const;

	static Quaternion fromEuler(Double3 euler);
	
	string str() const;

	Quaternion& operator += (const Quaternion& b);
	Quaternion& operator -= (const Quaternion& b);
	Quaternion& operator *= (const Quaternion& b);
	Quaternion& operator *= (double b);
};

Quaternion quaternionFromString(string s);

Quaternion FromToRotation(const Quaternion& a, const Quaternion& b);
Quaternion FromToRotation(const Double3& a, const Double3& b);

Quaternion operator + (const Quaternion& a, const Quaternion& b);
Quaternion operator - (const Quaternion& a, const Quaternion& b);
Quaternion operator * (const Quaternion& a, const Quaternion& b);
Double3 operator * (const Double3& a, const Quaternion& b);
Quaternion operator * (const Quaternion& a, double b);

#endif