#pragma once
#include <cmath>
#include <iostream>

#include "Common.h"

class Vec3
{
public:
	Vec3() : e{0, 0, 0}
	{
	}

	Vec3(double e0, double e1, double e2) : e{e0, e1, e2}
	{
	}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	Vec3 operator-() const { return {-e[0], -e[1], -e[2]}; }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double Length() const
	{
		return sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	bool NearZero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

	double e[3];
};

using Point3 = Vec3; // 3D point
using Color = Vec3; // RGB color

// Utility Functions
inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return {u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return {u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return {u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]};
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return {t * v.e[0], t * v.e[1], t * v.e[2]};
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t)
{
	return 1 / t * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return {
		u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]
	};
}

inline Vec3 UnitVector(Vec3 v)
{
	return v / v.Length();
}

static Vec3 Random()
{
	return {RandomDouble(), RandomDouble(), RandomDouble()};
}

static Vec3 Random(const double min, const double max)
{
	return {RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)};
}

inline Vec3 RandomInUnitSphere()
{
	while (true)
	{
		auto p = Random(-1, 1);
		if (p.LengthSquared() >= 1) continue;
		return p;
	}
}

inline Vec3 RandomInHemisphere(const Vec3& normal)
{
	const Vec3 in_unit_sphere = RandomInUnitSphere();
	if (Dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	return -in_unit_sphere;
}

inline Vec3 RandomCosineDirection()
{
	const auto r1 = RandomDouble();
	const auto r2 = RandomDouble();
	const auto z = sqrt(1 - r2);

	const auto phi = 2 * pi * r1;
	const auto x = cos(phi) * sqrt(r2);
	const auto y = sin(phi) * sqrt(r2);

	return {x, y, z};
}

inline Vec3 RandomUnitVector()
{
	return UnitVector(RandomInUnitSphere());
}

inline Vec3 Reflect(const Vec3& d, const Vec3& n)
{
	return d - 2 * Dot(d, n) * n;
}

inline double Distance(const Vec3 point1, const Vec3 point2)
{
	const double dx = point1.x() - point2.x();
	const double dy = point1.y() - point2.y();
	const double dz = point1.z() - point2.z();

	return std::sqrt(dx * dx + dy * dy + dz * dz);
}
