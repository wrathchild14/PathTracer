#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
	HittableList()
	= default;

	explicit HittableList(const shared_ptr<Hittable>& object) { add(object); }

	void clear() { objects.clear(); }
	void add(const shared_ptr<Hittable>& object) { objects.push_back(object); }

	bool Hit(
		const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;

public:
	std::vector<shared_ptr<Hittable>> objects;
};
