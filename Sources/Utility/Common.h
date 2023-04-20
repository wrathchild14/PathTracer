#pragma once
#include <cmath>
#include <limits>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double DegreesToRadians(const double degrees)
{
	return degrees * pi / 180.0;
}

inline double RandomDouble()
{
	// Returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * RandomDouble();
}

inline double Clamp(const double x, const double min, const double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

// Common Headers
#include "Ray.h"
#include "Vec3.h"
