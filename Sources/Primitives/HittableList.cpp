﻿#include "HittableList.h"


bool HittableList::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects_)
	{
		if (object->Hit(ray, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

double HittableList::PdfValue(const Point3& o, const Vec3& v) const
{
	const auto weight = 1.0 / objects_.size();
	auto sum = 0.0;

	for (const auto& object : objects_)
		sum += weight * object->PdfValue(o, v);

	return sum;
}

Vec3 HittableList::Random(const Vec3& o) const
{
	const auto int_size = static_cast<int>(objects_.size());
	return objects_[RandomInt(0, int_size - 1)]->Random(o);
}

std::vector<Label> HittableList::GetLabels(const Camera& camera)
{
	for (const auto& object : objects_)
	{
		if (typeid(object) == typeid(Sphere))
		{
			std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(object);

		}
	}
	return {};
}
