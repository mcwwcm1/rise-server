// File: double2.cpp
// Purpose: Implements Double2

#include "Double2.h"

#include <algorithm>

#include "MysticMath.h"

Double2::Double2(double x, double y)
{
	this->x = x;
	this->y = y;
}

Double2::Double2()
{
	x = 0;
	y = 0;
}

double Double2::MagnitudeSquared() const { return x * x + y * y; }

double Double2::Magnitude() const
{
	float squared = MagnitudeSquared();
	return squared == 0 ? 0 : sqrt(squared);
}

Double2 Double2::Normalized() const
{
	double m = Magnitude();
	return m == 0 ? Double2() : Double2(x / m, y / m);
}

Double2 Double2::Absolute() const { return *this * this->Sign(); }

Double2 Double2::Sign() const
{
	return Double2(x > 0, y > 0) - Double2(x < 0, y < 0);
}

Double2 Double2::Round() const
{
	return Double2(std::round(x), std::round(y));
}

bool Double2::IsUniform() const { return x == y; }

std::string Double2::ToString() const
{
	std::stringstream ss;
	ss << "[" << x << ";" << y << "]";
	return ss.str();
}

Double2 Double2FromString(const std::string& s)
{
	size_t sep1 = s.find(';');
	double x    = std::stof(s.substr(s.find('[') + 1, sep1 - 1));
	double y    = std::stof(s.substr(sep1 + 1, s.length() - 1));
	return Double2(x, y);
}

bool TryDouble2FromString(const std::string& s, Double2& result)
{
	try {
		result = Double2FromString(s);
		return true;
	} catch (const std::exception& e) {
		return false;
	}
}

double Dot(const Double2& a, const Double2& b)
{
	return a.x * b.x + a.y * b.y;
}

Double2 Reflect(const Double2& vec, const Double2& norm)
{
	return vec - (norm * 2 * Dot(vec, norm));
}

Double2 Clamp(const Double2& vec, const Double2& min, const Double2& max)
{
	return Double2(std::min(std::max(vec.x, min.x), max.x),
	               std::min(std::max(vec.y, min.y), max.y));
}

Double2 Lerp(const Double2& a, const Double2& b, double t)
{
	return a * (1 - t) + b * t;
}

// Assignment operator overloads
Double2& Double2::operator+=(const Double2& b)
{
	*this = *this + b;
	return *this;
}

Double2& Double2::operator-=(const Double2& b)
{
	*this = *this - b;
	return *this;
}

Double2& Double2::operator*=(const Double2& b)
{
	*this = *this * b;
	return *this;
}

Double2& Double2::operator*=(const double& b)
{
	*this = *this * b;
	return *this;
}

Double2& Double2::operator/=(const Double2& b)
{
	*this = *this / b;
	return *this;
}

Double2& Double2::operator/=(const double& b)
{
	*this = *this / b;
	return *this;
}

// Operator overloads
Double2 operator+(const Double2& a, const Double2& b)
{
	return Double2(a.x + b.x, a.y + b.y);
}

Double2 operator-(const Double2& a, const Double2& b)
{
	return Double2(a.x - b.x, a.y - b.y);
}

Double2 operator*(const Double2& a, const Double2& b)
{
	return Double2(a.x * b.x, a.y * b.y);
}

Double2 operator*(const Double2& a, const double& b)
{
	return Double2(a.x * b, a.y * b);
}

Double2 operator/(const Double2& a, const Double2& b)
{
	return Double2(a.x / b.x, a.y / b.y);
}

Double2 operator/(const Double2& a, const double& b)
{
	return Double2(a.x / b, a.y / b);
}