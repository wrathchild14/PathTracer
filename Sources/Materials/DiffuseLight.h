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

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered, double& pdf) const override
	{
		return false;
	}


	Color Emitted(const Point3& p) const override { return emit_; }

	Color Emitted(const Ray& r_in, const HitRecord& rec, double u, double v,
	              const Point3& p) const override
	{
		if (rec.FrontFace)
			return p;
		return {0, 0, 0};
	}

private:
	Color emit_;
};
