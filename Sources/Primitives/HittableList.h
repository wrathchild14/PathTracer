#pragma once

#include "Hittable.h"
#include "../Utility/Ray.h"

#include <memory>
#include <vector>

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

private:
	std::vector<std::shared_ptr<Hittable>> objects_;
};
