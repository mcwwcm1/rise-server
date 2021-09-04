// File: mysticmath.cpp
// Purpose: Implements general math functions

#include "MysticMath.h"

float DegreesToRadians(float degrees) { return degrees * DEG2RAD; }

float RadiansToDegrees(float radians) { return radians * RAD2DEG; }

float Lerp(float a, float b, float f) { return a + f * (b - a); }