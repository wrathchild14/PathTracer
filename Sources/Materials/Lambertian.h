#pragma once
#include "Material.h"
#include "../Utility/Ray.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo(a)
	{
	}

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		auto scatter_direction = rec.normal + RandomUnitVector();
		if (scatter_direction.NearZero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.point, scatter_direction);
		attenuation = albedo;
		return true;
	}

public:
	Color albedo;
};
