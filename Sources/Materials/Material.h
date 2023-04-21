#pragma once
#include "../Primitives/Hittable.h"

struct HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const
	{
		return false;
	}

	virtual double ScatteringPdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const
	{
		return 0;
	}

	virtual Color Emitted(const Point3& p) const
	{
		return {0, 0, 0};
	}

	virtual Color Emitted(const Ray& r_in, const HitRecord& rec, double u, double v,
	                      const Point3& p) const
	{
		return {0, 0, 0};
	}
};
