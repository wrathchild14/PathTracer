#pragma once

class HittablePdf : public Pdf
{
public:
	HittablePdf(const std::shared_ptr<Hittable> p, const Point3& origin) : o_(origin), ptr_(p)
	{
	}

	double Value(const Vec3& direction) const override
	{
		return ptr_->PdfValue(o_, direction);
	}

	Vec3 Generate() const override
	{
		return ptr_->Random(o_);
	}

private:
	Point3 o_;
	std::shared_ptr<Hittable> ptr_;
};

class CosinePdf : public Pdf
{
public:
	CosinePdf(const Vec3& w) { uvw_.BuildFromW(w); }

	double Value(const Vec3& direction) const override
	{
		const auto cosine = Dot(UnitVector(direction), uvw_.w());
		return cosine <= 0 ? 0 : cosine / pi;
	}

	Vec3 Generate() const override
	{
		return uvw_.local(RandomCosineDirection());
	}

private:
	ONB uvw_;
};

class MixturePdf : public Pdf
{
public:
	MixturePdf(std::shared_ptr<Pdf> pdf0, std::shared_ptr<Pdf> pdf1)
	{
		pdfs_[0] = std::move(pdf0);
		pdfs_[1] = std::move(pdf1);
	}

	double Value(const Vec3& direction) const override
	{
		return 0.5 * pdfs_[0]->Value(direction) + 0.5 * pdfs_[1]->Value(direction);
	}

	Vec3 Generate() const override
	{
		if (RandomDouble() < 0.5)
			return pdfs_[0]->Generate();
		return pdfs_[1]->Generate();
	}

private:
	std::shared_ptr<Pdf> pdfs_[2];
};

class OrenNayarPdf : public Pdf
{
public:
	OrenNayarPdf(const Vec3& w, const double roughness) : roughness_(roughness)
	{
		uvw_.BuildFromW(w);
	}

	double Value(const Vec3& direction) const override
	{
		const auto cosine = Dot(UnitVector(direction), uvw_.w());
		if (cosine <= 0)
		{
			return 0;
		}

		const auto sine = std::sqrt(1 - cosine * cosine);
		const auto sin_alpha = sine / std::max(cosine, 1e-6);
		// const auto tan_beta = roughness_ * sin_alpha;

		const auto A = 1.0 - 0.5 * roughness_ * roughness_ / (roughness_ * roughness_ + 0.33);
		const auto B = 0.45 * roughness_ * roughness_ / (roughness_ * roughness_ + 0.09);

		const auto pdf_value = cosine * (A + B * std::max(0.0, Dot(direction, uvw_.u()))
			* sin_alpha * std::tan(std::acos(cosine)));
		return pdf_value / pi;
	}

	Vec3 Generate() const override
	{
		return uvw_.local(RandomOrenNayarDirection(roughness_));
	}

private:
	ONB uvw_;
	double roughness_;

	Vec3 RandomOrenNayarDirection(const double roughness) const
	{
		const double u = RandomDouble();
		const double v = RandomDouble();

		const double phi = 2 * pi * u;
		const double cos_theta = std::sqrt((1 - v) * (1 - v) * (1 - roughness * roughness) + v);
		const double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

		const auto local_direction = Vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
		return local_direction;
	}
};
