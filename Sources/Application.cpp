#include "Application.h"

#include "Camera.h"
#include "Lambertian.h"
#include "Material.h"


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
	auto R = cos(pi / 4);

	auto material_left = make_shared<Lambertian>(Color(0, 0, 1));
	auto material_right = make_shared<Lambertian>(Color(1, 0, 0));

	world.add(make_shared<Sphere>(Point3(-R, 0, -1), R, material_left));
	world.add(make_shared<Sphere>(Point3(R, 0, -1), R, material_right));

	const int samples_per_pixel = 40;
	const int max_depth = 20;

	const Camera camera(120, aspect_ratio_);

	for (int j = height_ - 1; j >= 0; --j)
	{
		for (int i = 0; i < width_; ++i)
		{
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				const auto u = (i + RandomDouble()) / (width_ - 1);
				const auto v = (j + RandomDouble()) / (height_ - 1);
				Ray r = camera.GetRay(u, v);
				pixel_color += RayColor(r, world, max_depth);
			}

			auto r = pixel_color.x();
			auto g = pixel_color.y();
			auto b = pixel_color.z();
			const auto scale = 1.0 / samples_per_pixel;
			r = sqrt(scale * r);
			g = sqrt(scale * g);
			b = sqrt(scale * b);

			image_[(j * width_ + i) * 3] = static_cast<int>(256 * Clamp(r, 0.0, 0.999));
			image_[(j * width_ + i) * 3 + 1] = static_cast<int>(256 * Clamp(g, 0.0, 0.999));
			image_[(j * width_ + i) * 3 + 2] = static_cast<int>(256 * Clamp(b, 0.0, 0.999));
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

Color Application::RayColor(const Ray& ray, const Hittable& world, const int depth)
{
	if (depth <= 0)
		return {0, 0, 0};
	HitRecord rec;
	if (world.Hit(ray, 0.001, infinity, rec))
	{
		Ray scattered;
		Color attenuation;
		if (rec.material->Scatter(ray, rec, attenuation, scattered))
			return attenuation * RayColor(scattered, world, depth - 1);
		return {0, 0, 0};
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
