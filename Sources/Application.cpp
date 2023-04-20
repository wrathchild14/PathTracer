#include "Application.h"


Application::Application(const int width, const double aspect_ratio): width_(width),
                                                                      height_(static_cast<int>(width / aspect_ratio)),
                                                                      aspect_ratio_(aspect_ratio)
{
	image_ = new unsigned char[height_ * width_ * 3];
}

Application::Application(const int width, const int height)
	: width_(width), height_(height), aspect_ratio_(2)
{
	image_ = new unsigned char[height_ * width_ * 3];
}

Application::~Application()
{
	free(image_);
}

void Application::Render() const
{
	// Go in Init() please :)
	HittableList world;
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	const auto viewport_height = 2.0;
	const auto viewport_width = aspect_ratio_ * viewport_height;
	const auto focal_length = 1.0;

	const auto origin = Point3(0, 0, 0);
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
			Color pixel_color = RayColor(r, world);

			image_[(j * width_ + i) * 3] = static_cast<int>(255.999 * pixel_color.x());
			image_[(j * width_ + i) * 3 + 1] = static_cast<int>(255.999 * pixel_color.y());
			image_[(j * width_ + i) * 3 + 2] = static_cast<int>(255.999 * pixel_color.z());
		}
	}
}

double Application::HitSphere(const Point3& center, const double radius, const Ray& r)
{
	const Vec3 oc = r.Origin() - center;
	const auto a = r.Direction().LengthSquared();
	const auto half_b = Dot(oc, r.Direction());
	const auto c = oc.LengthSquared() - radius * radius;
	const auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0)
	{
		return -1.0;
	}
	return (-half_b - sqrt(discriminant)) / a;
}

Color Application::RayColor(const Ray& ray, const Hittable& world)
{
	HitRecord rec;
	if (world.Hit(ray, 0, infinity, rec))
	{
		return 0.5 * (rec.normal + Color(1, 1, 1));
	}
	const Vec3 unit_direction = UnitVector(ray.Direction());
	const auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
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
