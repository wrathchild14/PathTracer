#pragma once
#include "Ray.h"
#include "Vec3.h"

class Camera
{
public:
	Camera(const Point3& look_from, const Point3& look_at, const Vec3& up, const double fov,
	       const double aspect_ratio) : origin_(look_from), look_at_(look_at), up_(up), fov_(fov),
	                                    aspect_ratio_(aspect_ratio)
	{
		const auto theta = DegreesToRadians(fov);
		const auto h = tan(theta / 2);
		const auto viewport_height = 2.0 * h;
		const auto viewport_width = aspect_ratio * viewport_height;

		const auto w = UnitVector(look_from - look_at);
		const auto u = UnitVector(Cross(up, w));
		const auto v = Cross(w, u);

		horizontal_ = viewport_width * u;
		vertical_ = viewport_height * v;
		lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - w;
	}

	Ray GetRay(const double s, const double t) const
	{
		return {origin_, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_};
	}

	Point3 GetPosition() const
	{
		return origin_;
	}

	double GetFov() const
	{
		return fov_;
	}

	double GetAspectRatio() const
	{
		return aspect_ratio_;
	}

	Vec3 GetUp() const
	{
		return up_;
	}

	Vec3 GetTarget() const
	{
		return look_at_;
	}

private:
	Point3 origin_{};
	Point3 lower_left_corner_{};
	Vec3 look_at_;
	Vec3 horizontal_{};
	Vec3 vertical_{};
	Vec3 up_;

	double fov_;
	double aspect_ratio_;
};
