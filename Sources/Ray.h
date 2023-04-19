#pragma once
#include "Vec3.h"

class Ray
{
public:
	Ray(const point3& origin, const Vec3& direction) : orig_(origin), dir_(direction) {}

	point3 Origin() const { return orig_; }
	Vec3 Direction() const { return dir_; }
	point3 At(double t) const { return orig_ + dir_ * t; }

public:
	point3 orig_;
	Vec3 dir_;
};
