// File: quaternion.h
// Purpose: For working with quaternions

#ifndef QUATERNION_H
#define QUATERNION_H

#include <string>
#include <sstream>
#include <math.h>
#include "float3.h"
#include "mysticmath.h"

using namespace std;

struct Quaternion
{
	public:
		float x, y, z, w;
		static const Quaternion identity; // Unsure on this


	Quaternion(float x, float y, float z, float w);
	Quaternion();
	
	Quaternion conjugate();
	Float3 toEuler();

	static Quaternion fromEuler(Float3 euler);
	
	string str();
};

Quaternion operator + (const Quaternion a, const Quaternion b);
Quaternion operator - (const Quaternion a, const Quaternion b);
Quaternion operator * (const Quaternion a, const Quaternion b);
Float3 operator * (const Float3 a, const Quaternion b);

#endif