#pragma once
#include "Hittable.h"
#include "Utility/Ray.h"

class Sphere : public Hittable
{
public:
	Sphere() = default;

	Sphere(const Point3& center, const double r) : center_(center), radius_(r)
	{
	}
	Sphere(const Point3 center, const double radius, std::shared_ptr<Material> material)
				: center_(center), radius_(radius), material_(std::move(material)) {};
	
	virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

private:
	Point3 center_;
	double radius_{};
	std::shared_ptr<Material> material_;
};
