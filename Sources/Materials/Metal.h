#pragma once

class Metal : public Material
{
public:
	Metal(const Color& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1)
	{
	}

	bool Scatter(
		const Ray& r_in, const HitRecord& rec, ScatterRecord& srec, const bool oren_nayar, const double roughness
	) const override
	{
		const Vec3 reflected = Reflect(UnitVector(r_in.Direction()), rec.normal);
		srec.specular_ray = Ray(rec.point, reflected + fuzz_ * RandomInUnitSphere());
		srec.attenuation = albedo_;
		srec.is_specular = true;
		srec.pdf = nullptr;
		return true;
	}

private:
	Color albedo_;
	double fuzz_;
};
