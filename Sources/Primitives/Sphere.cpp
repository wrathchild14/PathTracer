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

	auto root = (-half_b - sqrt_d) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrt_d) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.point = std::dynamic_pointer_cast<DiffuseLight>(material_) == nullptr ? ray.At(rec.t) : rec.point;
	const Vec3 outward_normal = (rec.point - center_) / radius_;
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material_;
	rec.point = std::dynamic_pointer_cast<DiffuseLight>(material_) != nullptr ? ray.At(rec.t) : rec.point;
	rec.is_sphere = true;

	return true;
}


double Sphere::PdfValue(const Point3& o, const Vec3& v) const
{
	HitRecord rec;
	if (!this->Hit(Ray(o, v), 0.001, infinity, rec))
		return 0;

	const auto cos_theta_max = sqrt(1 - radius_ * radius_ / (center_ - o).LengthSquared());
	const auto solid_angle = 2 * pi * (1 - cos_theta_max);

	return 1 / solid_angle;
}

Vec3 Sphere::Random(const Point3& o) const
{
	const Vec3 direction = center_ - o;
	const auto distance_squared = direction.LengthSquared();
	ONB uvw;
	uvw.BuildFromW(direction);
	return uvw.local(RandomToSphere(radius_, distance_squared));
}

Point3 Sphere::GetCenter() const
{
	return center_;
}

double Sphere::GetRadius() const
{
	return radius_;
}

Vec3 Sphere::RandomToSphere(const double radius, const double distance_squared) const
{
	const auto r1 = RandomDouble();
	const auto r2 = RandomDouble();
	const auto z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

	const auto phi = 2 * pi * r1;
	const auto x = cos(phi) * sqrt(1 - z * z);
	const auto y = sin(phi) * sqrt(1 - z * z);

	return {x, y, z};
}
