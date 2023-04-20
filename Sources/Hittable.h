#pragma once
#include "AABB.h"
#include "Material.h"
#include "Utility/Ray.h"

struct HitRecord
{
	Point3 point;
	Vec3 normal;
	std::shared_ptr<Material> material;
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
	virtual bool BoundingBox(double time0, double time1, AABB& output_box) const { return false; }
};
