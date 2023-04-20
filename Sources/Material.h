#pragma once
#include "Utility/Common.h"

struct hit_record;

class Material
{
public:
	virtual ~Material() = default;
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;

	virtual Color Emitted(const Point3& p) const
	{
		return {0, 0, 0};
	}
};
