#include "Application.h"

Application::Application() : width_(400), aspect_ratio_(1.0)
{
	height_ = static_cast<int>(width_ / aspect_ratio_);
	image_ = new unsigned char[height_ * width_ * 3];
	camera_ = new Camera(Point3(278, 278, -800), Point3(278, 278, 0), Vec3(0, 1, 0), 40, aspect_ratio_);
	world_ = std::make_shared<HittableList>();
	lights_ = std::make_shared<HittableList>();
	AddCornellBoxToWorld();
}

Application::Application(const int width, const double aspect_ratio) :
	width_(width), height_(static_cast<int>(width / aspect_ratio)), aspect_ratio_(aspect_ratio)
{
	image_ = new unsigned char[height_ * width_ * 3];
	camera_ = new Camera(Point3(278, 278, -800), Point3(278, 278, 0), Vec3(0, 1, 0), 40, aspect_ratio_);
	world_ = std::make_shared<HittableList>();
	lights_ = std::make_shared<HittableList>();
	AddCornellBoxToWorld();
}

void Application::SetWidth(const int width)
{
	width_ = width;
	height_ = static_cast<int>(width / aspect_ratio_);
	delete[] image_;
	image_ = new unsigned char[height_ * width_ * 3];
}

Application::~Application()
{
	free(image_);
}

void Application::AddCornellBoxToWorld() const
{
	auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
	auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
	auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

	world_->Add(std::make_shared<YZRectangle>(0, 555, 0, 555, 555, green));
	world_->Add(std::make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
	world_->Add(std::make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
	world_->Add(std::make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
	world_->Add(std::make_shared<XYRectangle>(0, 555, 0, 555, 555, white));

	world_->Add(std::make_shared<FlipFace>(std::make_shared<XZRectangle>(250, 300, 260, 320, 554, light)));
	lights_->Add(std::make_shared<XZRectangle>(250, 300, 260, 320, 554));
}

void Application::Render(const int j, const int samples_per_pixel, const int depth,
	const bool is_russian_roulette, const bool is_oren_nayar, const bool roughness) const
{
	for (int i = 0; i < width_; ++i)
	{
		// neural_net = MyNet("my_net.pt");
		// neural_net.predict(image);
		Color pixel_color(0, 0, 0);
		Color pixel;
		for (int s = 0; s < samples_per_pixel; ++s)
		{
			const auto u = (i + RandomDouble()) / (width_ - 1);
			const auto v = (j + RandomDouble()) / (height_ - 1);
			Ray r = camera_->GetRay(u, v);

			pixel_color += RayColor(r, background_, world_, lights_, depth, is_oren_nayar, roughness);

			if (s > samples_per_pixel * 30 / 100 && is_russian_roulette)
			{
				pixel = UnitVector(pixel_color);
				double rr_probability = std::max(pixel.x(), std::max(pixel.y(), pixel.z()));
				if (rr_probability < 0.1) rr_probability = 0.1;
				const auto random_double = RandomDouble();
				if (random_double > rr_probability)
					break;
			}
		}

		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();

		// Surface acne
		if (r != r) r = 0.0;
		if (g != g) g = 0.0;
		if (b != b) b = 0.0;

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

Color Application::RayColor(const Ray& ray, const Color& background, const std::shared_ptr<HittableList>& world,
	const std::shared_ptr<Hittable>& lights, const int depth, const bool is_oren_nayar,
	const double roughness)
{
	if (depth <= 0)
		return { 0, 0, 0 };
	HitRecord rec;

	if (!world->Hit(ray, 0.001, infinity, rec))
		return background;

	ScatterRecord s_rec;
	Color emitted;
	if (rec.material != nullptr)
	{
		emitted = rec.material->Emitted(ray, rec, rec.point);

		if (!rec.material->Scatter(ray, rec, s_rec, is_oren_nayar, roughness))
			return emitted;
	}

	if (s_rec.is_specular)
	{
		return s_rec.attenuation * RayColor(s_rec.specular_ray, background, world, lights, depth - 1, is_oren_nayar,
			roughness);
	}

	const auto light_ptr = std::make_shared<HittablePdf>(lights, rec.point);
	const MixturePdf mixture_pdf(light_ptr, s_rec.pdf);

	const auto scattered = Ray(rec.point, mixture_pdf.Generate());
	const auto pdf = mixture_pdf.Value(scattered.Direction());

	return emitted
		+ s_rec.attenuation * rec.material->ScatteringPdf(ray, rec, scattered)
		* RayColor(scattered, background, world, lights, depth - 1, is_oren_nayar, roughness) / pdf;
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

void Application::AddRandomSphere() const
{
	world_->Add(std::make_shared<Sphere>(Point3(RandomInt(0, 500), RandomInt(0, 1000), RandomInt(0, 500)),
		RandomInt(5, 100), RandomMaterial()));
}

std::shared_ptr<Material> Application::RandomMaterial() const
{
	// const double r = RandomDouble();
	// if (r < 1.0 / 3.0)
	// return std::make_shared<Glass>(RandomDouble(0.5, 1.5));
	// if (r < 2.0 / 3.0)
	// return std::make_shared<Metal>(Color(RandomDouble(), RandomDouble(), RandomDouble()), RandomDouble());
	return std::make_shared<Lambertian>(Color(RandomDouble(), RandomDouble(), RandomDouble()));
}

void Application::AddRandomSphereLight() const
{
	const int intensity = RandomInt(5, 20);
	const auto light = std::make_shared<DiffuseLight>(Color(intensity, intensity, intensity));
	const auto random_radius = RandomInt(5, 30);
	const auto random_point = Point3(RandomInt(0, 500), RandomInt(0, 500), RandomInt(0, 500));
	world_->Add(std::make_shared<FlipFace>(std::make_shared<Sphere>(random_point, random_radius, light)));
	lights_->Add(std::make_shared<Sphere>(random_point, random_radius));
}

void Application::CleanScene() const
{
	world_->Clear();
	lights_->Clear();
	AddCornellBoxToWorld();
}
