#include "Rectangle.h"

bool XZRectangle::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	const auto t = (k_ - ray.Origin().y()) / ray.Direction().y();
	if (t < t_min || t > t_max)
		return false;
	const auto x = ray.Origin().x() + t * ray.Direction().x();
	const auto z = ray.Origin().z() + t * ray.Direction().z();
	if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(0, 1, 0);
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material_;
	rec.point = ray.At(t);
	return true;
}

bool YZRectangle::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	const auto t = (k_ - ray.Origin().x()) / ray.Direction().x();
	if (t < t_min || t > t_max)
		return false;
	const auto y = ray.Origin().y() + t * ray.Direction().y();
	const auto z = ray.Origin().z() + t * ray.Direction().z();
	if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(1, 0, 0);
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material_;
	rec.point = ray.At(t);
	return true;
}

bool XYRectangle::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	const auto t = (k_ - ray.Origin().z()) / ray.Direction().z();
	if (t < t_min || t > t_max)
		return false;
	const auto x = ray.Origin().x() + t * ray.Direction().x();
	const auto y = ray.Origin().y() + t * ray.Direction().y();
	if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
		return false;
	rec.t = t;
	const auto outward_normal = Vec3(0, 0, 1);
	rec.SetFaceNormal(ray, outward_normal);
	rec.material = material_;
	rec.point = ray.At(t);
	return true;
}
