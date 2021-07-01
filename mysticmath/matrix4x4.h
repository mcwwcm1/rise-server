// File: matrix4x4.h
// Purpose: For defining and working with 4x4 matrices

#ifndef MATRIX_H
#define MATRIX_H

#include "float3.h"
#include "quaternion.h"

struct Matrix4x4
{
	Matrix4x4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15);
	Matrix4x4();

	public:
		float v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15;
		static const Matrix4x4 identity;
	
	float * operator [] (int index);
	
	Matrix4x4 translate(Float3 delta);
	Matrix4x4 rotate(Quaternion rotation);
	Matrix4x4 scale(Float3 v);
};

Matrix4x4 getTranslationMatrix(Float3 translation);
Matrix4x4 getRotationMatrix(Quaternion rotation);
Matrix4x4 getScaleMatrix(Float3 scale);

Matrix4x4 getTRSMatrix(Float3 position, Quaternion rotation, Float3 scale);

Matrix4x4 operator * (Matrix4x4 m, float f);
Float3 operator * (Float3 v, Matrix4x4 m);
Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2);

#endif