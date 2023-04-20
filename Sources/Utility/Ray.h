#pragma once
#include "Vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : orig_(origin), dir_(direction)
	{
	}

	Point3 Origin() const { return orig_; }
	Vec3 Direction() const { return dir_; }
	Point3 At(double t) const { return orig_ + dir_ * t; }

private:
	Point3 orig_;
	Vec3 dir_;
};
