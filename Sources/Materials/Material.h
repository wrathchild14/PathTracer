#pragma once
#include <memory>

#include "Pdf.h"

struct HitRecord;

struct ScatterRecord
{
	Ray specular_ray;
	bool is_specular;
	Color attenuation;
	std::shared_ptr<Pdf> pdf;
};

class Material
{
public:
	virtual ~Material() = default;

	virtual Color Emitted(const Ray& r_in, const HitRecord& rec, const Point3& p) const
	{
		return {0, 0, 0};
	}

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec, const bool oren_nayar,
	                     const double roughness) const
	{
		return false;
	}

	virtual double ScatteringPdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const
	{
		return 0;
	}
};
