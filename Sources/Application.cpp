#include "Application.h"


Application::Application(int width, double aspect_ratio): width_(width),
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

void Application::Render() const
{
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio_ * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = Vec3(viewport_width, 0, 0);
	auto vertical = Vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

	for (int j = 0; j < width_; j++)
	{
		for (int i = 0; i < height_; i++)
		{
			// auto u = static_cast<double>(i) / (width_ - 1);
			// auto v = static_cast<double>(j) / (height_ - 1);
			// Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			// color pixel_color = RayColor(r);

			color pixel_color(static_cast<double>(i) / (width_ - 1), static_cast<double>(j) / (height_ - 1), 0.25);

			// auto r = static_cast<double>(i) / (width_ - 1);
			// auto g = static_cast<double>(j) / (height_ - 1);
			// auto b = 0.25;

			int ir = static_cast<int>(255.999 * pixel_color.x());
			int ig = static_cast<int>(255.999 * pixel_color.y());
			int ib = static_cast<int>(255.999 * pixel_color.z());

			// int ir = static_cast<int>(255.999 * r);
			// int ig = static_cast<int>(255.999 * g);
			// int ib = static_cast<int>(255.999 * b);


			image_[(j * width_ + i) * 3] = ir;
			image_[(j * width_ + i) * 3 + 1] = ig;
			image_[(j * width_ + i) * 3 + 2] = ib;
		}
	}
}

color Application::RayColor(const Ray& r)
{
	Vec3 unit_direction = UnitVector(r.Direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
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
