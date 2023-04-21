#pragma once
#include <utility>

#include "../Utility/OrthonomalBasis.h"
#include "../Utility/Vec3.h"

class Pdf
{
public:
	virtual ~Pdf() = default;

	virtual double Value(const Vec3& direction) const = 0;
	virtual Vec3 Generate() const = 0;
};

inline Vec3 RandomCosineDirection()
{
	const auto r1 = RandomDouble();
	const auto r2 = RandomDouble();
	const auto z = sqrt(1 - r2);

	const auto phi = 2 * pi * r1;
	const auto x = cos(phi) * sqrt(r2);
	const auto y = sin(phi) * sqrt(r2);

	return {x, y, z};
}

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

class HittablePdf : public Pdf
{
public:
	HittablePdf(std::shared_ptr<Hittable> p, const Point3& origin) : o_(origin), ptr_(std::move(p))
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
