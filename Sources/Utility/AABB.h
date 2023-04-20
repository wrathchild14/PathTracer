#pragma once
#include "Ray.h"

class AABB
{
public:
	AABB()
	= default;

	AABB(const Point3& a, const Point3& b)
	{
		minimum = a;
		maximum = b;
	}

	Point3 min() const { return minimum; }
	Point3 max() const { return maximum; }

	bool hit(const Ray& r, double t_min, double t_max) const
	{
		for (int a = 0; a < 3; a++)
		{
			const auto inv_d = 1.0f / r.Direction()[a];
			auto t0 = (min()[a] - r.Origin()[a]) * inv_d;
			auto t1 = (max()[a] - r.Origin()[a]) * inv_d;
			if (inv_d < 0.0f)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)
				return false;
		}
		return true;
	}

	Point3 minimum;
	Point3 maximum;
};
