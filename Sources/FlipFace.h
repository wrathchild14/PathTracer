#pragma once
#include <memory>

#include "Primitives/Hittable.h"
#include "Utility/Ray.h"

class FlipFace : public Hittable
{
public:
	FlipFace(std::shared_ptr<Hittable> p) : ptr(std::move(p))
	{
	}

	bool Hit(
		const Ray& r, const double t_min, const double t_max, HitRecord& rec) const override
	{
		if (!ptr->Hit(r, t_min, t_max, rec))
			return false;

		rec.FrontFace = !rec.FrontFace;
		return true;
	}

public:
	std::shared_ptr<Hittable> ptr;
};
