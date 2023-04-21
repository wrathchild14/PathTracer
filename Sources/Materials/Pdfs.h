#pragma once
#include "../Primitives/Hittable.h"

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
