#pragma once
#include "HittableList.h"

class Material;

class Sphere : public Hittable
{
public:
	Sphere() = default;

	Sphere(const Point3& center, const double r) : center_(center), radius_(r)
	{
	}

	Sphere(const Point3 center, const double radius, std::shared_ptr<Material> material)
		: center_(center), radius_(radius), material_(std::move(material))
	{
	}

	bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

private:
	Vec3 RandomToSphere(double radius, double distance_squared) const;

	Point3 center_;
	double radius_{};
	std::shared_ptr<Material> material_;
};
