#include "Sphere.h"


bool Sphere::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
	Vec3 oc = ray.Origin() - center_;
	auto a = ray.Direction().LengthSquared();
	auto half_b = Dot(oc, ray.Direction());
	auto c = oc.LengthSquared() - radius_ * radius_;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.point = ray.At(rec.t);
	const Vec3 outward_normal = (rec.point - center_) / radius_;
	rec.SetFaceNormal(ray, outward_normal);

	return true;
}
