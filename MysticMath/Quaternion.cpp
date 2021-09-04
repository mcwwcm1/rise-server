// File: quaternion.cpp
// Purpose: Implements quaternion.h

#include "Quaternion.h"

#include "Utilities.h"

Quaternion::Quaternion(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion()
{
	x = y = z = 0;
	w         = 1;
}
const Quaternion Quaternion::identity = Quaternion(0, 0, 0, 1);

double Quaternion::MagnitudeSquared() const
{
	return x * x + y * y + z * z + w * w;
}

double Quaternion::Magnitude() const { return sqrt(MagnitudeSquared()); }

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(x * -1, y * -1, z * -1, w);
}

Quaternion Quaternion::Normalized() const { return *this * Magnitude(); }

Double3 Quaternion::ToEuler() const
{
	Double3 euler;

	double sinr_cosp = 2 * (w * x + y * z);
	double cosr_cosp = 1 - 2 * (x * x + y * y);
	euler.x          = std::atan2(sinr_cosp, cosr_cosp);

	double sinp = 2 * (w * y - z * x);
	if (std::abs(sinp) >= 1)
		euler.y = std::copysignf(PI / 2, sinp);
	else
		euler.y = std::asin(sinp);

	double siny_cosp = 2 * (w * z + x * y);
	double cosy_cosp = 1 - 2 * (y * y + z * z);
	euler.z          = std::atan2(siny_cosp, cosy_cosp);

	euler = euler * RAD2DEG;

	return euler;
}

std::string Quaternion::ToString() const
{
	std::stringstream ss;
	ss << "[" << x << ";" << y << ";" << z << ";" << w << "]";
	return ss.str();
}

Quaternion Quaternion::FromEuler(Double3 euler)
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

Quaternion QuaternionFromString(const std::string& s)
{
	auto parts = Split(s.substr(1, s.length() - 1), ';');
	if (parts.size() != 4) {
		throw std::invalid_argument("Invalid number of parts for quaternion, expected 4");
	}
	return Quaternion(
			stof(parts[0]), stof(parts[1]), stof(parts[2]), stof(parts[3]));
}

bool TryQuaternionFromString(const std::string& s, Quaternion& result)
{
	try {
		result = QuaternionFromString(s);
		return true;
	} catch (const std::exception& e) {
		return false;
	}
}

Quaternion FromToRotation(const Quaternion& a, const Quaternion& b)
{
	return a.Conjugate() * b;
}

Quaternion FromToRotation(const Double3& a, const Double3& b)
{
	Double3 c = Cross(a, b);
	return Quaternion(
						 c.x,
						 c.y,
						 c.z,
						 sqrt(a.MagnitudeSquared() * b.MagnitudeSquared()) + Dot(a, b))
	    .Normalized();
}

Quaternion::operator btQuaternion()
{
	return btQuaternion(x, y, z, w);
}

Quaternion& Quaternion::operator=(const btQuaternion& b)
{
	this->x = b.getX();
	this->y = b.getY();
	this->z = b.getZ();
	this->w = b.getW();
	return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& b)
{
	*this = *this + b;
	return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& b)
{
	*this = *this - b;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& b)
{
	*this = *this * b;
	return *this;
}

Quaternion& Quaternion::operator*=(double b)
{
	*this = *this * b;
	return *this;
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Quaternion operator-(const Quaternion& a, const Quaternion& b)
{
	return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Quaternion operator*(const Quaternion& a, const Quaternion& b)
{
	return Quaternion((a.x * b.w + b.x * a.w + a.y * b.z) - (a.z * b.y),
	                  (a.y * b.w + b.y * a.w + a.z * b.x) - (a.x * b.z),
	                  (a.z * b.w + b.z * a.w + a.x * b.y) - (a.y * b.x),
	                  (a.w * b.w) - (a.x * b.x + a.y * b.y + a.z * b.z));
}

Double3 operator*(const Double3& a, const Quaternion& b)
{
	Quaternion c = Quaternion(a.x, a.y, a.z, 0);
	Quaternion r = b * c * b.Conjugate();
	return Double3(r.x, r.y, r.z);
}

Quaternion operator*(const Quaternion& a, double b)
{
	return Quaternion(a.x * b, a.y * b, a.z * b, a.w * b);
}
