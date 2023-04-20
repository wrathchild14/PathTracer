#include "Sphere.h"


bool Sphere::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	const Vec3 oc = ray.Origin() - center_;
	const auto a = ray.Direction().LengthSquared();
	const auto half_b = Dot(oc, ray.Direction());
	const auto c = oc.LengthSquared() - radius_ * radius_;

	const auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	const auto sqrt_d = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrt_d) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrt_d) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.point = ray.At(rec.t);
	const Vec3 outward_normal = (rec.point - center_) / radius_;
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material_;

	return true;
}
