#pragma once
#include "Material.h"
#include "../Utility/Ray.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo((a))
	{
	}

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double &pdf) const override
	{
		auto scatter_direction = rec.normal + RandomUnitVector();
		if (scatter_direction.NearZero())
			scatter_direction = rec.normal;
		
		scattered = Ray(rec.point, UnitVector(scatter_direction));
		attenuation = albedo;
		pdf = Dot(rec.normal, scattered.Direction()) / pi;
		return true;
	}

	double ScatteringPdf(
			const Ray& r_in, const HitRecord& rec, const Ray& scattered
		) const {
		auto cosine = Dot(rec.normal, UnitVector(scattered.Direction()));
		return cosine < 0 ? 0 : cosine/pi;
	}

public:
	mutable Color albedo;
};
