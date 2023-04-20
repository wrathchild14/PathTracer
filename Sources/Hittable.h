#pragma once
#include "Utility/Ray.h"
#include "Utility/Vec3.h"

struct HitRecord
{
	Point3 point;
	Vec3 normal;
	double t{};

	bool FrontFace{};

	void SetFaceNormal(const Ray& ray, const Vec3& outward_normal)
	{
		FrontFace = Dot(ray.Direction(), outward_normal) < 0;
		normal = FrontFace ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};
