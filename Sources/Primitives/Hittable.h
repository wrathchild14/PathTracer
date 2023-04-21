#pragma once
#include "../Utility/AABB.h"
#include "../Materials/Material.h"
#include "../Utility/Ray.h"

struct HitRecord
{
	Point3 point;
	Vec3 normal;
	std::shared_ptr<Material> material;
	double t{};
	bool front_face{};

	void SetFaceNormal(const Ray& ray, const Vec3& outward_normal)
	{
		front_face = Dot(ray.Direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
	virtual bool BoundingBox(double time0, double time1, AABB& output_box) const { return false; }

	virtual double PdfValue(const Point3& o, const Vec3& v) const
	{
		return 0.0;
	}

	virtual Vec3 Random(const Vec3& o) const
	{
		return {1, 0, 0};
	}
};
