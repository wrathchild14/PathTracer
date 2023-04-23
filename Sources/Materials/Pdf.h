#pragma once
#include "../Utility/OrthonomalBasis.h"

class Pdf
{
public:
	virtual ~Pdf() = default;

	virtual double Value(const Vec3& direction) const = 0;
	virtual Vec3 Generate() const = 0;
};
