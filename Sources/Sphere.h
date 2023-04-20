#pragma once
#include "Hittable.h"
#include "Utility/Ray.h"

class Sphere : public Hittable
{
public:
	Sphere() = default;

	Sphere(const Point3 center, const double r) : center_(center), radius_(r)
	{
	}

	virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

private:
	Point3 center_;
	double radius_{};
};
