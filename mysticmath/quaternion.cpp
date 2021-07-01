// File: quaternion.cpp
// Purpose: Implements quaternion.h

#include "quaternion.h"

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion()
{
	x = 1;
}
const Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);

Quaternion Quaternion::conjugate()
{
	return Quaternion(x * -1, y * -1, z * -1, w);
}

Float3 Quaternion::toEuler()
{
	Float3 euler;

	float sinr_cosp = 2 * (w * x + y * z);
	float cosr_cosp = 1 - 2 * (x * x + y * y);
	euler.x = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = 2 * (w * y - z * x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysignf(PI / 2, sinp);
	else
		euler.y = std::asin(sinp);

	float siny_cosp = 2 * (w * z + x * y);
	float cosy_cosp = 1 - 2 * (y * y + z * z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

	euler = euler * RAD2DEG;

	return euler;
}

string Quaternion::str()
{
	stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << ";" << w << "]";
	return ss.str();
}

Quaternion Quaternion::fromEuler(Float3 euler)
{
	euler = euler * DEG2RAD;
	
    // Abbreviations for the various angular functions
    double cp = cosf(euler.x * 0.5);
    double sp = sinf(euler.x * 0.5);
	double cy = cosf(euler.y * 0.5);
    double sy = sinf(euler.y * 0.5);
    double cr = cosf(euler.z * 0.5);
    double sr = sinf(euler.z * 0.5);

    Quaternion q;
    q.w = cp * cy * cr + sp * sy * sr;
    q.x = sp * cy * cr - cp * sy * sr;
    q.y = cp * sy * cr + sp * cy * sr;
    q.z = cp * cy * sr - sp * sy * cr;

    return q;
}

Quaternion operator + (Quaternion a, Quaternion b)
{
	return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Quaternion operator - (Quaternion a, Quaternion b)
{
	return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Quaternion operator * (Quaternion a, Quaternion b)
{
	return Quaternion(	a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x,
						-a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y,
						a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z,
						-a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w);
}

Float3 operator * (Float3 a, Quaternion b)
{
	Quaternion c = Quaternion(a.x, a.y, a.z, 0);
	Quaternion r = b * c * b.conjugate();
	return Float3(r.x, r.y, r.z);
}