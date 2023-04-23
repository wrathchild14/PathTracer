#pragma once
#include <memory>

class FlipFace : public Hittable
{
public:
	FlipFace(std::shared_ptr<Hittable> p) : ptr_(std::move(p))
	{
	}

	bool Hit(
		const Ray& r, const double t_min, const double t_max, HitRecord& rec) const override
	{
		if (!ptr_->Hit(r, t_min, t_max, rec))
			return false;

		rec.front_face = !rec.front_face;
		return true;
	}

private:
	std::shared_ptr<Hittable> ptr_;
};
