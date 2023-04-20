#include "Application.h"


Application::Application(const int width, const double aspect_ratio): width_(width),
                                                                      height_(static_cast<int>(width / aspect_ratio)),
                                                                      aspect_ratio_(aspect_ratio)
{
	image_ = new unsigned char[height_ * width_ * 3];
	world_ = GetCBExample();
	camera_ = new Camera(Point3(278, 278, -800), Point3(278, 278, 0), Vec3(0, 1, 0), 40, aspect_ratio_);
	max_depth_ = 20;
}

Application::~Application()
{
	free(image_);
}

HittableList Application::GetCBExample() const
{
	HittableList objects;

	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));
	auto pink = std::make_shared<Lambertian>(Color(0.96, 0.06, 0.84));
	auto weird_green = std::make_shared<Lambertian>(Color(0.29, 0.95, 0.67));

	objects.Add(std::make_shared<Sphere>(Point3(150, 80, 300), 100, pink));
	objects.Add(std::make_shared<Sphere>(Point3(330, 50, 100), 50, weird_green));

	// Cornell Box
	objects.Add(std::make_shared<YZRectangle>(0, 555, 0, 555, 555, green));
	objects.Add(std::make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
	objects.Add(std::make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
	objects.Add(std::make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
	objects.Add(std::make_shared<XYRectangle>(0, 555, 0, 555, 555, white));
	objects.Add(std::make_shared<XZRectangle>(213, 343, 227, 332, 554, light));

	return objects;
}

void Application::Render(const int j, const int samples_per_pixel) const
{
	for (int i = 0; i < width_; ++i)
	{
		Color pixel_color(0, 0, 0);
		for (int s = 0; s < samples_per_pixel; ++s)
		{
			const auto u = (i + RandomDouble()) / (width_ - 1);
			const auto v = (j + RandomDouble()) / (height_ - 1);
			Ray r = camera_->GetRay(u, v);
			pixel_color += RayColor(r, background_, world_, max_depth_);
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

Color Application::RayColor(const Ray& ray, const Color& background, const Hittable& world, const int depth)
{
	if (depth <= 0)
		return {0, 0, 0};
	HitRecord rec;

	if (!world.Hit(ray, 0.001, infinity, rec))
		return background;

	Ray scattered;
	Color attenuation;
	const Color emmited = rec.material->Emitted(rec.point);

	if (!rec.material->Scatter(ray, rec, attenuation, scattered))
		return emmited;

	return emmited + attenuation * RayColor(scattered, background, world, depth - 1);
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
