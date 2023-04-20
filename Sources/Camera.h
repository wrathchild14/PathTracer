#pragma once
#include "Utility/Ray.h"

class Camera
{
public:
	Camera()
	{
		const auto aspect_ratio = 16.0 / 9.0;
		const auto viewport_height = 2.0;
		const auto viewport_width = aspect_ratio * viewport_height;
		const auto focal_length = 1.0;

		origin_ = Point3(0, 0, 0);
		horizontal_ = Vec3(viewport_width, 0.0, 0.0);
		vertical_ = Vec3(0.0, viewport_height, 0.0);
		lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - Vec3(0, 0, focal_length);
	}

	Ray GetRay(const double u, const double v) const
	{
		return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
	}

private:
	Point3 origin_{};
	Point3 lower_left_corner_{};
	Vec3 horizontal_{};
	Vec3 vertical_{};
};
