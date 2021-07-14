// File: double3.h
// Purpose: Used for vector math

#ifndef DOUBLE3_H
#define DOUBLE3_H

#include <string>
#include <math.h>
#include <sstream>

using namespace std;

struct Double3
{
	public:
		double x, y, z;

		Double3(double x, double y, double z);
		Double3();

		double magnitude();
		double magnitudeSquared();
		Double3 normalized();
		Double3 absolute();
		Double3 sign();
		Double3 round();
		bool isUniform();

		string str();

		// Assignment operator overloads
		Double3& operator += (const Double3& b);
		Double3& operator -= (const Double3& b);
		Double3& operator *= (const Double3& b);
		Double3& operator *= (const double& b);
		Double3& operator /= (const Double3& b);
		Double3& operator /= (const double& b);
};

double dot(const Double3& a, const Double3& b);
Double3 cross(const Double3& a, const Double3& b);
Double3 tripleProduct(const Double3& a, const Double3& b, const Double3& c);
Double3 reflect(const Double3& vec, const Double3& norm);
Double3 clamp(const Double3& vec, const Double3& min, const Double3& max);

// Operator overloads
Double3 operator + (const Double3& a, const Double3& b);
Double3 operator - (const Double3& a, const Double3& b);
Double3 operator * (const Double3& a, const Double3& b);
Double3 operator * (const Double3& a, const double& b);
Double3 operator / (const Double3& a, const Double3& b);
Double3 operator / (const Double3& a, const double& b);

#endif