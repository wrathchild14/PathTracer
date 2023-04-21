#pragma once
#include "../Utility/OrthonomalBasis.h"

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
