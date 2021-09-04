// File: matrix4x4.h
// Purpose: For defining and working with 4x4 matrices

#pragma once

#include "Double3.h"
#include "Quaternion.h"

struct Matrix4x4 {
	Matrix4x4(double v0,
	          double v1,
	          double v2,
	          double v3,
	          double v4,
	          double v5,
	          double v6,
	          double v7,
	          double v8,
	          double v9,
	          double v10,
	          double v11,
	          double v12,
	          double v13,
	          double v14,
	          double v15);
	Matrix4x4();

 public:
	double v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15;
	static const Matrix4x4 identity;

	double* operator[](int index);

	Matrix4x4 Translate(Double3 delta);
	Matrix4x4 Rotate(Quaternion rotation);
	Matrix4x4 Scale(Double3 v);

	std::string ToString();
};

Matrix4x4 GetTranslationMatrix(Double3 translation);
Matrix4x4 GetRotationMatrix(Quaternion rotation);
Matrix4x4 GetScaleMatrix(Double3 scale);

Matrix4x4 GetTRSMatrix(Double3 position, Quaternion rotation, Double3 scale);

Matrix4x4 operator*(Matrix4x4 m, double f);
Double3 operator*(Double3 v, Matrix4x4 m);
Matrix4x4 operator*(Matrix4x4 m1, Matrix4x4 m2);