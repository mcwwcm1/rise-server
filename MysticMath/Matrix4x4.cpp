// File: matrix4x4.cpp
// Purpose: Implements matrix4x4.h

#include "Matrix4x4.h"

Matrix4x4::Matrix4x4(double v0,
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
                     double v15)
{
	this->v0  = v0;
	this->v1  = v1;
	this->v2  = v2;
	this->v3  = v3;
	this->v4  = v4;
	this->v5  = v5;
	this->v6  = v6;
	this->v7  = v7;
	this->v8  = v8;
	this->v9  = v9;
	this->v10 = v10;
	this->v11 = v11;
	this->v12 = v12;
	this->v13 = v13;
	this->v14 = v14;
	this->v15 = v15;
}

Matrix4x4::Matrix4x4() { *this = identity; }

const Matrix4x4 Matrix4x4::identity =
		Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

double* Matrix4x4::operator[](int index)
{
	switch (index) {
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

Matrix4x4 Matrix4x4::Translate(Double3 delta)
{
	*this = *this * GetTranslationMatrix(delta);
	return *this;
}

Matrix4x4 Matrix4x4::Rotate(Quaternion rotation)
{
	*this = *this * GetRotationMatrix(rotation);
	return *this;
}

Matrix4x4 Matrix4x4::Scale(Double3 v)
{
	*this = *this * GetScaleMatrix(v);
	return *this;
}

std::string Matrix4x4::ToString()
{
	std::stringstream ss;
	ss << "[";
	ss << v0 << ";" << v1 << ";" << v2 << ";" << v3 << ";";
	ss << v4 << ";" << v5 << ";" << v6 << ";" << v7 << ";";
	ss << v8 << ";" << v9 << ";" << v10 << ";" << v11 << ";";
	ss << v12 << ";" << v13 << ";" << v14 << ";" << v15 << ";";
	ss << "]";
	return ss.str();
}

Matrix4x4 GetTranslationMatrix(Double3 translation)
{
	return Matrix4x4(1,
	                 0,
	                 0,
	                 translation.x,
	                 0,
	                 1,
	                 0,
	                 translation.y,
	                 0,
	                 0,
	                 1,
	                 translation.z,
	                 0,
	                 0,
	                 0,
	                 1);
}

Matrix4x4 GetRotationMatrix(Quaternion rotation)
{
	double x = rotation.x;
	double y = rotation.y;
	double z = rotation.z;
	double w = rotation.w;

	double xx = 2 * x * x;
	double yy = 2 * y * y;
	double zz = 2 * z * z;

	double xy = 2 * x * y;
	double wz = 2 * w * z;
	double xz = 2 * x * z;
	double wy = 2 * w * y;
	double yz = 2 * y * z;
	double wx = 2 * w * x;

	return Matrix4x4((1 - yy - zz),
	                 (xy - wz),
	                 (xz + wy),
	                 0,
	                 (xy + wz),
	                 (1 - xx - zz),
	                 (yz - wx),
	                 0,
	                 (xz - wy),
	                 (yz + wx),
	                 (1 - xx - yy),
	                 0,
	                 0,
	                 0,
	                 0,
	                 1);
}

Matrix4x4 GetScaleMatrix(Double3 scale)
{
	return Matrix4x4(
			scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1);
}

Matrix4x4 GetTRSMatrix(Double3 position, Quaternion rotation, Double3 scale)
{
	return GetTranslationMatrix(position) * GetRotationMatrix(rotation) *
	       GetScaleMatrix(scale);
}

Matrix4x4 operator*(Matrix4x4 m, double f)
{
	return Matrix4x4(m.v0 * f,
	                 m.v1 * f,
	                 m.v2 * f,
	                 m.v3 * f,
	                 m.v4 * f,
	                 m.v5 * f,
	                 m.v6 * f,
	                 m.v7 * f,
	                 m.v8 * f,
	                 m.v9 * f,
	                 m.v10 * f,
	                 m.v11 * f,
	                 m.v12 * f,
	                 m.v13 * f,
	                 m.v14 * f,
	                 m.v15 * f);
}

Double3 operator*(Double3 vec, Matrix4x4 m)
{
	Double3 r;
	r.x = vec.x * *m[0] + vec.y * *m[1] + vec.z * *m[2] + *m[3];
	r.y = vec.x * *m[4] + vec.y * *m[5] + vec.z * *m[6] + *m[7];
	r.z = vec.x * *m[8] + vec.y * *m[9] + vec.z * *m[10] + *m[11];
	return r;
}

Matrix4x4 operator*(Matrix4x4 m1, Matrix4x4 m2)
{
	return Matrix4x4(
			m1.v0 * m2.v0 + m1.v1 * m2.v4 + m1.v2 * m2.v8 + m1.v3 * m2.v12,
			m1.v0 * m2.v1 + m1.v1 * m2.v5 + m1.v2 * m2.v9 + m1.v3 * m2.v13,
			m1.v0 * m2.v2 + m1.v1 * m2.v6 + m1.v2 * m2.v10 + m1.v3 * m2.v14,
			m1.v0 * m2.v3 + m1.v1 * m2.v7 + m1.v2 * m2.v11 + m1.v3 * m2.v15,
			m1.v4 * m2.v0 + m1.v5 * m2.v4 + m1.v6 * m2.v8 + m1.v7 * m2.v12,
			m1.v4 * m2.v1 + m1.v5 * m2.v5 + m1.v6 * m2.v9 + m1.v7 * m2.v13,
			m1.v4 * m2.v2 + m1.v5 * m2.v6 + m1.v6 * m2.v10 + m1.v7 * m2.v14,
			m1.v4 * m2.v3 + m1.v5 * m2.v7 + m1.v6 * m2.v11 + m1.v7 * m2.v15,
			m1.v8 * m2.v0 + m1.v9 * m2.v4 + m1.v10 * m2.v8 + m1.v11 * m2.v12,
			m1.v8 * m2.v1 + m1.v9 * m2.v5 + m1.v10 * m2.v9 + m1.v11 * m2.v13,
			m1.v8 * m2.v2 + m1.v9 * m2.v6 + m1.v10 * m2.v10 + m1.v11 * m2.v14,
			m1.v8 * m2.v3 + m1.v9 * m2.v7 + m1.v10 * m2.v11 + m1.v11 * m2.v15,
			m1.v12 * m2.v0 + m1.v13 * m2.v4 + m1.v14 * m2.v8 + m1.v15 * m2.v12,
			m1.v12 * m2.v1 + m1.v13 * m2.v5 + m1.v14 * m2.v9 + m1.v15 * m2.v13,
			m1.v12 * m2.v2 + m1.v13 * m2.v6 + m1.v14 * m2.v10 + m1.v15 * m2.v14,
			m1.v12 * m2.v3 + m1.v13 * m2.v7 + m1.v14 * m2.v11 + m1.v15 * m2.v15);
}