#pragma once

class Glass : public Material
{
public:
	Glass(const double index) : index_of_refraction_(index)
	{
	}

	bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& s_rec, const bool oren_nayar, const double roughness) const override
	{
		s_rec.is_specular = true;
		s_rec.pdf = nullptr;
		s_rec.attenuation = Color(1.0, 1.0, 1.0);
		const double refraction_ratio = rec.front_face ? 1.0 / index_of_refraction_ : index_of_refraction_;

		const Vec3 unit_direction = UnitVector(r_in.Direction());
		const double cos_theta = fmin(Dot(-unit_direction, rec.normal), 1.0);
		const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		Vec3 direction;
		if (cannot_refract || ReflectanceFresnel(cos_theta, refraction_ratio) > RandomDouble())
			direction = Reflect(unit_direction, rec.normal);
		else
			direction = RefractSnell(unit_direction, rec.normal, refraction_ratio);

		s_rec.specular_ray = Ray(rec.point, direction);
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
	double index_of_refraction_;

	static double ReflectanceFresnel(const double cosine, const double ref_idx)
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}

	static Vec3 RefractSnell(const Vec3& uv, const Vec3& n, const double etai_over_etat)
	{
		const auto cos_theta = fmin(Dot(-uv, n), 1.0);
		const Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
		const Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.LengthSquared())) * n;
		return r_out_perp + r_out_parallel;
	}
};
