#pragma once
#include "Material.h"
#include "Pdfs.h"
#include "../Utility/Ray.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo_(a)
	{
	}

	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec) const override
	{
		s_rec.is_specular = false;
		s_rec.attenuation = albedo_;
		s_rec.pdf = std::make_shared<CosinePdf>(rec.normal);
		return true;
	}

	double ScatteringPdf(
		const Ray& r_in, const HitRecord& rec, const Ray& scattered
	) const override
	{
		const auto cosine = Dot(rec.normal, UnitVector(scattered.Direction()));
		return cosine < 0 ? 0 : cosine / pi;
	}

private:
	mutable Color albedo_;
};
