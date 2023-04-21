#pragma once
#include <memory>

#include "Hittable.h"
#include "../Utility/Ray.h"
#include "../Materials/Material.h"

class XZRectangle : public Hittable
{
public:
	XZRectangle()
	= default;

	XZRectangle(const double x0, const double x1, const double z0, const double z1, const double k)
		: x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k)
	{
	}

	XZRectangle(const double x0, const double x1, const double z0, const double z1, const double k,
	            std::shared_ptr<Material> mat)
		: material_(std::move(mat)), x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k)
	{
	}

	bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	bool BoundingBox(double time0, double time1, AABB& output_box) const override
	{
		// The bounding box must have non-zero width in each dimension, so pad the Y
		// dimension a small amount.
		output_box = AABB(Point3(x0_, k_ - 0.0001, z0_), Point3(x1_, k_ + 0.0001, z1_));
		return true;
	}

	double PdfValue(const Point3& origin, const Vec3& v) const override
	{
		HitRecord rec;
		if (!this->Hit(Ray(origin, v), 0.001, infinity, rec))
			return 0;

		const auto area = (x1_ - x0_) * (z1_ - z0_);
		const auto distance_squared = rec.t * rec.t * v.LengthSquared();
		const auto cosine = fabs(Dot(v, rec.normal) / v.Length());

		return distance_squared / (cosine * area);
	}

	Vec3 Random(const Point3& origin) const override
	{
		const auto random_point = Point3(RandomDouble(x0_, x1_), k_, RandomDouble(z0_, z1_));
		return random_point - origin;
	}

private:
	std::shared_ptr<Material> material_;
	double x0_{}, x1_{}, z0_{}, z1_{}, k_{};
};

class YZRectangle : public Hittable
{
public:
	YZRectangle()
	= default;

	YZRectangle(const double y0, const double y1, const double z0, const double z1, const double k)
		: y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k)
	{
	}

	YZRectangle(const double y0, const double y1, const double z0, const double z1, const double k,
	            std::shared_ptr<Material> material)
		: material_(std::move(material)), y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k)
	{
	}

	bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	bool BoundingBox(double time0, double time1, AABB& output_box) const override
	{
		// The bounding box must have non-zero width in each dimension, so pad the X
		// dimension a small amount.
		output_box = AABB(Point3(k_ - 0.0001, y0_, z0_), Point3(k_ + 0.0001, y1_, z1_));
		return true;
	}

	double PdfValue(const Point3& origin, const Vec3& v) const override
	{
		HitRecord rec;
		if (!this->Hit(Ray(origin, v), 0.001, infinity, rec))
			return 0;

		const auto area = (y1_ - y0_) * (z1_ - z0_);
		const auto distance_squared = rec.t * rec.t * v.LengthSquared();
		const auto cosine = fabs(Dot(v, rec.normal) / v.Length());

		return distance_squared / (cosine * area);
	}

	Vec3 Random(const Point3& origin) const override
	{
		const auto random_point = Point3(k_, RandomDouble(y0_, y1_), RandomDouble(z0_, z1_));
		return random_point - origin;
	}

private:
	std::shared_ptr<Material> material_;
	double y0_{}, y1_{}, z0_{}, z1_{}, k_{};
};

class XYRectangle : public Hittable
{
public:
	XYRectangle()
	= default;

	XYRectangle(const double x0, const double x1, const double y0, const double y1, const double k,
	            std::shared_ptr<Material> material)
		: material_(std::move(material)), x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k)
	{
	}

	bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	bool BoundingBox(double time0, double time1, AABB& output_box) const override
	{
		// The bounding box must have non-zero width in each dimension, so pad the Z
		// dimension a small amount.
		output_box = AABB(Point3(x0_, y0_, k_ - 0.0001), Point3(x1_, y1_, k_ + 0.0001));
		return true;
	}

private:
	std::shared_ptr<Material> material_;
	double x0_{}, x1_{}, y0_{}, y1_{}, k_{};
};
