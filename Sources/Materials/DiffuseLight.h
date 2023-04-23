#pragma once
#include "../Utility/Vec3.h"
#include "../Materials/Material.h"

class Ray;

class DiffuseLight : public Material
{
public:
	explicit DiffuseLight(const Color c) : emit_(c)
	{
	}

	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec, const bool oren_nayar, double roughness) const override
	{
		return false;
	}

	Color Emitted(const Ray& r_in, const HitRecord& rec, const Point3& p) const override
	{
		if (rec.front_face)
			return p;
		return {0, 0, 0};
	}

private:
	Color emit_;
};
