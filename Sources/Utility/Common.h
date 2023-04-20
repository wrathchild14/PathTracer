#pragma once
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double DegreesToRadians(const double degrees)
{
	return degrees * pi / 180.0;
}

inline double RandomDouble()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(const double min, const double max)
{
	return min + (max - min) * RandomDouble();
}

inline double Clamp(const double x, const double min, const double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
