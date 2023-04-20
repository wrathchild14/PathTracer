#include "Rectangle.h"

bool XZRectangle::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
	const auto t = (k_ - r.Origin().y()) / r.Direction().y();
	if (t < t_min || t > t_max)
		return false;
	auto x = r.Origin().x() + t * r.Direction().x();
	auto z = r.Origin().z() + t * r.Direction().z();
	if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(0, 1, 0);
	rec.SetFaceNormal(r, outward_normal);
	rec.material = material_;
	rec.point = r.At(t);
	return true;
}

bool YZRectangle::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
	const auto t = (k_ - r.Origin().x()) / r.Direction().x();
	if (t < t_min || t > t_max)
		return false;
	auto y = r.Origin().y() + t * r.Direction().y();
	auto z = r.Origin().z() + t * r.Direction().z();
	if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(1, 0, 0);
	rec.SetFaceNormal(r, outward_normal);
	rec.material = material_;
	rec.point = r.At(t);
	return true;
}

bool XYRectangle::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
	const auto t = (k_ - r.Origin().z()) / r.Direction().z();
	if (t < t_min || t > t_max)
		return false;
	const auto x = r.Origin().x() + t * r.Direction().x();
	const auto y = r.Origin().y() + t * r.Direction().y();
	if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(0, 0, 1);
	rec.SetFaceNormal(r, outward_normal);
	rec.material = material_;
	rec.point = r.At(t);
	return true;
}
