#pragma once

#include "Hittable.h"

#include "Sphere.h"
#include "../Utility/Camera.h"
#include <vector>

struct Label
{
	int object_class;
	double x;
	double y;
	double width;
	double height;
};

class HittableList : public Hittable
{
public:
	HittableList()
	= default;

	explicit HittableList(const std::shared_ptr<Hittable>& object) { Add(object); }

	void Clear() { objects_.clear(); }
	void Add(const std::shared_ptr<Hittable>& object) { objects_.push_back(object); }

	bool Hit(
		const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
	double PdfValue(const Point3& o, const Vec3& v) const override;
	Vec3 Random(const Vec3& o) const override;

	std::vector<Label> GetSphereLabels(const Camera& camera, const int& width, const int& height) const;

private:
	std::vector<std::shared_ptr<Hittable>> objects_;
};
