#pragma once
#include "Hittable.h"
#include "Material.h"

class DiffuseLight : public Material
{
public:
	explicit DiffuseLight(const Color c) : emit(c)
	{
	}

	bool Scatter(
		const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
	) const override
	{
		return false;
	}

	Color Emitted(const Point3& p) const override
	{
		return emit;
	}

public:
	Color emit;
};
