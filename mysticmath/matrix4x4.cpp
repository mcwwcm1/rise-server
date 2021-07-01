// File: matrix4x4.cpp
// Purpose: Implements matrix4x4.h

#include "matrix4x4.h"

Matrix4x4::Matrix4x4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15)
{
	this->v0 = v0;
	this->v1 = v1;
	this->v2 = v2;
	this->v3 = v3;
	this->v4 = v4;
	this->v5 = v5;
	this->v6 = v6;
	this->v7 = v7;
	this->v8 = v8;
	this->v9 = v9;
	this->v10 = v10;
	this->v11 = v11;
	this->v12 = v12;
	this->v13 = v13;
	this->v14 = v14;
	this->v15 = v15;
}

Matrix4x4::Matrix4x4()
{
	*this = Matrix4x4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
}

const Matrix4x4 Matrix4x4::identity = Matrix4x4(1,0,0,0,
												0,1,0,0,
												0,0,1,0,
												0,0,0,1);

float * Matrix4x4::operator[](int index)
{
	switch(index)
	{
		case 0:
			return &v0;
		case 1:
			return &v1;
		case 2:
			return &v2;
		case 3:
			return &v3;
		case 4:
			return &v4;
		case 5:
			return &v5;
		case 6:
			return &v6;
		case 7:
			return &v7;
		case 8:
			return &v8;
		case 9:
			return &v9;
		case 10:
			return &v10;
		case 11:
			return &v11;
		case 12:
			return &v12;
		case 13:
			return &v13;
		case 14:
			return &v14;
		case 15:
			return &v15;
	}

	throw 0;
}

Matrix4x4 Matrix4x4::translate(Float3 delta)
{
	*this = *this * getTranslationMatrix(delta);
	return *this;
}

Matrix4x4 Matrix4x4::rotate(Quaternion rotation)
{
	*this = *this * getRotationMatrix(rotation);
	return *this;
}

Matrix4x4 Matrix4x4::scale(Float3 v)
{
	*this = *this * getScaleMatrix(v);
	return *this;
}

Matrix4x4 getTranslationMatrix(Float3 translation)
{
	return Matrix4x4(	1,0,0,translation.x,
						0,1,0,translation.y,
						0,0,1,translation.z,
						0,0,0,1);
}

Matrix4x4 getRotationMatrix(Quaternion rotation)
{
	Float3 radEuler = rotation.toEuler() * DEG2RAD;

	Matrix4x4 x = Matrix4x4(
		1, 0, 0, 0,
		0, cosf(radEuler.x), sinf(radEuler.x), 0,
		0, -sinf(radEuler.x), cosf(radEuler.x), 0,
		0, 0, 0, 1
	);

	Matrix4x4 y = Matrix4x4(
		cosf(radEuler.y), 0, -sinf(radEuler.y), 0,
		0, 1, 0, 0,
		sinf(radEuler.y), 0, cosf(radEuler.y), 0,
		0, 0, 0, 1
	);

	Matrix4x4 z = Matrix4x4(
		cosf(radEuler.z), -sinf(radEuler.z), 0, 0,
		sinf(radEuler.z), cosf(radEuler.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	return x * y * z;
}

Matrix4x4 getScaleMatrix(Float3 scale)
{
	return Matrix4x4(	scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
				0, 0, scale.z, 0,
				0, 0, 0, 1);
}

Matrix4x4 getTRSMatrix(Float3 position, Quaternion rotation, Float3 scale)
{
	return getScaleMatrix(scale) * getRotationMatrix(rotation) * getTranslationMatrix(position);
}

Matrix4x4 operator * (Matrix4x4 m, float f)
{
	return Matrix4x4(	m.v0 * f, 	m.v1 * f, 	m.v2 * f, 	m.v3 * f,
							m.v4 * f, 	m.v5 * f, 	m.v6 * f, 	m.v7 * f,
							m.v8 * f, 	m.v9 * f, 	m.v10 * f, 	m.v11 * f,
							m.v12 * f, 	m.v13 * f, 	m.v14 * f, 	m.v15 * f);
}

Float3 operator * (Float3 vec, Matrix4x4 m)
{
	Float3 r;
	r.x = vec.x * *m[0] + vec.y * *m[1] + vec.z * *m[2] + *m[3];
	r.y = vec.x * *m[4] + vec.y * *m[5] + vec.z * *m[6] + *m[7];
	r.z = vec.x * *m[8] + vec.y * *m[9] + vec.z * *m[10] + *m[11];
	return r;
}

Matrix4x4 operator * (Matrix4x4 m1, Matrix4x4 m2)
{
	Matrix4x4 r;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int i = 0; i < 4; i++)
			{
				*r[y*4 + x] += *m1[y*4 + i] * *m2[i*4 + x]; 
			}
		}
	}
	
	return r;
}