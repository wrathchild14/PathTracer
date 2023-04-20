#pragma once
#include "Utility/Ray.h"

class Camera
{
public:
	Camera(Point3 look_from, Point3 look_at, Vec3 up, const double fov, const double aspect_ratio)
	{
		const auto theta = DegreesToRadians(fov);
		const auto h = tan(theta / 2);
		const auto viewport_height = 2.0 * h;
		const auto viewport_width = aspect_ratio * viewport_height;

		const auto w = UnitVector(look_from - look_at);
		const auto u = UnitVector(Cross(up, w));
		const auto v = Cross(w, u);

		origin_ = look_from;
		horizontal_ = viewport_width * u;
		vertical_ = viewport_height * v;
		lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - w;
	}

	Ray GetRay(const double s, const double t) const
	{
		return Ray(origin_, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_);
	}

private:
	Point3 origin_{};
	Point3 lower_left_corner_{};
	Vec3 horizontal_{};
	Vec3 vertical_{};
};
