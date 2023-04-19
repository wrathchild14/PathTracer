#include "Application.h"


Application::Application(const int width, const double aspect_ratio): width_(width),
                                                                      height_(static_cast<int>(width / aspect_ratio)),
                                                                      aspect_ratio_(aspect_ratio)
{
	image_ = new unsigned char[height_ * width_ * 3];
}

Application::Application(const int width, int height)
	: width_(width), height_(height), aspect_ratio_(0)
{
	image_ = new unsigned char[height_ * width_ * 3];
}

Application::~Application()
{
	free(image_);
}

void Application::Render() const
{
	const auto viewport_height = 2.0;
	const auto viewport_width = aspect_ratio_ * viewport_height;
	const auto focal_length = 1.0;

	const auto origin = point3(0, 0, 0);
	const auto horizontal = Vec3(viewport_width, 0, 0);
	const auto vertical = Vec3(0, viewport_height, 0);
	const auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

	for (int j = height_ - 1; j >= 0; --j)
	{
		for (int i = 0; i < width_; ++i)
		{
			auto u = static_cast<double>(i) / (width_ - 1);
			auto v = static_cast<double>(j) / (height_ - 1);
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = RayColor(r);

			int ir = static_cast<int>(255.999 * pixel_color.x());
			int ig = static_cast<int>(255.999 * pixel_color.y());
			int ib = static_cast<int>(255.999 * pixel_color.z());

			image_[(j * width_ + i) * 3] = ir;
			image_[(j * width_ + i) * 3 + 1] = ig;
			image_[(j * width_ + i) * 3 + 2] = ib;
		}
	}
}

bool Application::HitSphere(const point3& center, const double radius, const Ray& r)
{
	const Vec3 oc = r.Origin() - center;
	const auto a = Dot(r.Direction(), r.Direction());
	const auto b = 2.0 * Dot(oc, r.Direction());
	const auto c = Dot(oc, oc) - radius * radius;
	const auto discriminant = b * b - 4 * a * c;
	return discriminant > 0;
}

color Application::RayColor(const Ray& r)
{
	if (HitSphere(point3(0, 0, -1), 0.6, r))
		return {1, 0, 0};
	const Vec3 unit_direction = UnitVector(r.Direction());
	const auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

unsigned char* Application::GetImage() const
{
	if (image_ != nullptr)
	{
		return image_;
	}
	return nullptr;
}

int Application::GetImageWidth() const
{
	return width_;
}

int Application::GetImageHeight() const
{
	return height_;
}
