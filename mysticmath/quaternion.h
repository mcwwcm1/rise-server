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
	
	Quaternion conjugate() const;
	Double3 toEuler();

	static Quaternion fromEuler(Double3 euler);
	
	string str();

	Quaternion& operator += (const Quaternion& b);
	Quaternion& operator -= (const Quaternion& b);
	Quaternion& operator *= (const Quaternion& b);
};

Quaternion operator + (const Quaternion& a, const Quaternion& b);
Quaternion operator - (const Quaternion& a, const Quaternion& b);
Quaternion operator * (const Quaternion& a, const Quaternion& b);
Double3 operator * (const Double3& a, const Quaternion& b);

#endif