// File: double3.cpp
// Purpose: Implements Double3

#include "double3.h"

#include <algorithm>

#include "mysticmath.h"

Double3::Double3(double x, double y, double z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

Double3::Double3()
{
  x = 0;
  y = 0;
  z = 0;
}

double Double3::magnitudeSquared() const { return x * x + y * y + z * z; }

double Double3::magnitude() const
{
  float squared = magnitudeSquared();
  return squared == 0 ? 0 : sqrt(squared);
}

Double3 Double3::normalized() const
{
  double m = magnitude();
  return m == 0 ? Double3() : Double3(x / m, y / m, z / m);
}

Double3 Double3::absolute() const { return *this * this->sign(); }

Double3 Double3::sign() const
{
  return Double3(x > 0, y > 0, z > 0) - Double3(x < 0, y < 0, z < 0);
}

Double3 Double3::round() const
{
  return Double3(std::round(x), std::round(y), std::round(z));
}

bool Double3::isUniform() const { return x == y && y == z; }

string Double3::str() const
{
  stringstream ss;
  ss << "[" << x << ";" << y << ";" << z << "]";
  return ss.str();
}

Double3 double3FromString(string s)
{
  size_t sep1 = s.find(';');
  size_t sep2 = s.find(';', sep1 + 1);
  double x    = stof(s.substr(s.find('[') + 1, sep1 - 1));
  double y    = stof(s.substr(sep1 + 1, sep2 - sep1));
  double z    = stof(s.substr(sep2 + 1, s.length() - sep2 - 1));
  return Double3(x, y, z);
}

bool tryDouble3FromString(string s, Double3& result)
{
  try {
    result = double3FromString(s);
    return true;
  } catch (exception e) {
    return false;
  }
}

double dot(const Double3& a, const Double3& b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Double3 cross(const Double3& a, const Double3& b)
{
  return Double3(
      a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Double3 tripleProduct(const Double3& a, const Double3& b, const Double3& c)
{
  return cross(a, cross(b, c));
}

Double3 reflect(const Double3& vec, const Double3& norm)
{
  return vec - (norm * 2 * dot(vec, norm));
}

Double3 clamp(const Double3& vec, const Double3& min, const Double3& max)
{
  return Double3(std::min(std::max(vec.x, min.x), max.x),
                 std::min(std::max(vec.y, min.y), max.y),
                 std::min(std::max(vec.z, min.z), max.z));
}

Double3 lerp(const Double3& a, const Double3& b, double t)
{
  return a * (1 - t) + b * t;
}

// Assignment operator overloads
Double3& Double3::operator+=(const Double3& b)
{
  *this = *this + b;
  return *this;
}

Double3& Double3::operator-=(const Double3& b)
{
  *this = *this - b;
  return *this;
}

Double3& Double3::operator*=(const Double3& b)
{
  *this = *this * b;
  return *this;
}

Double3& Double3::operator*=(const double& b)
{
  *this = *this * b;
  return *this;
}

Double3& Double3::operator/=(const Double3& b)
{
  *this = *this / b;
  return *this;
}

Double3& Double3::operator/=(const double& b)
{
  *this = *this / b;
  return *this;
}

// Operator overloads
Double3 operator+(const Double3& a, const Double3& b)
{
  return Double3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Double3 operator-(const Double3& a, const Double3& b)
{
  return Double3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Double3 operator*(const Double3& a, const Double3& b)
{
  return Double3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Double3 operator*(const Double3& a, const double& b)
{
  return Double3(a.x * b, a.y * b, a.z * b);
}

Double3 operator/(const Double3& a, const Double3& b)
{
  return Double3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Double3 operator/(const Double3& a, const double& b)
{
  return Double3(a.x / b, a.y / b, a.z / b);
}