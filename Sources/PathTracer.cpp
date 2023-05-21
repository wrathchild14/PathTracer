#include "PathTracer.h"

PathTracer::PathTracer(uint8_t* image, const double aspect_ratio)
{
	image_ = image;
	camera_ = new Camera(Point3(278, 278, -800), Point3(278, 278, 0), Vec3(0, 1, 0), 40, aspect_ratio);
	world_ = std::make_shared<HittableList>();
	lights_ = std::make_shared<HittableList>();
	AddCornellBoxToWorld();
}

void PathTracer::SetImageWidth(const int width)
{
	image_width_ = width;
	image_height_ = static_cast<int>(width);
	delete[] image_;
	image_ = new uint8_t[image_height_ * image_width_ * 3];
}

uint8_t* PathTracer::GetImage() const
{
	if (image_ != nullptr)
	{
		return image_;
	}
	return nullptr;
}

int PathTracer::GetImageWidth() const
{
	return image_width_;
}

int PathTracer::GetImageHeight() const
{
	return image_height_;
}

bool PathTracer::IsInScreenBoxes(const int i, const int j) const
{
	const auto boxes = GetSphereScreenBoxes();
	for (const auto& box : boxes)
	{
		const auto min_x = box.x - box.width;
		const auto max_x = box.x + box.width;

		const auto max_y = image_height_ - (box.y - box.height);
		const auto min_y = image_height_ - (box.y + box.height);

		const auto horizontal_side = box.x;
		const auto vertical_side = image_height_ - box.y;
		const auto radius = box.width;

		if (min_x < i && i < max_x && min_y < j && j < max_y)
		{
			const double distance = std::sqrt(
				(i - horizontal_side) * (i - horizontal_side) + (j - vertical_side) * (j - vertical_side));
			if (distance <= radius)
			{
				// (i, j) falls within the circle
				return true;
			}
		}
	}
	return false;
}

void PathTracer::Render(const int i, const int j, int samples_per_pixel, const int depth,
                        const bool is_oren_nayar, const double roughness, const bool focusing,
                        const int focus_multiplier, const bool importance_sampling) const
{
	Color pixel_color(0, 0, 0);
	const auto inv_samples_per_pixel = 1.0 / samples_per_pixel;
	const auto scale = inv_samples_per_pixel / sqrt(samples_per_pixel);

	if (focusing)
	{
		const auto u = (i + RandomDouble()) / (image_width_ - 1);
		const auto v = (j + RandomDouble()) / (image_height_ - 1);
		HitRecord test_record;
		const Ray test_ray = camera_->GetRay(u, v);
		if (world_->Hit(test_ray, 0.001, infinity, test_record))
			if (test_record.is_main) // && IsInScreenBoxes(i, j))
				samples_per_pixel *= focus_multiplier;
	}

	if (importance_sampling)
	{
		const auto u = (i + RandomDouble()) / (image_width_ - 1);
		const auto v = (j + RandomDouble()) / (image_height_ - 1);
		const Ray ray = camera_->GetRay(u, v);
		pixel_color += RayColorImportanceSampling(ray, background_, world_, lights_, depth, is_oren_nayar, roughness,
		                                          samples_per_pixel);
	}
	else
	{
		for (int s = 0; s < samples_per_pixel; ++s)
		{
			const auto u = (i + RandomDouble()) / (image_width_ - 1);
			const auto v = (j + RandomDouble()) / (image_height_ - 1);
			Ray r = camera_->GetRay(u, v);
			pixel_color += RayColor(r, background_, world_, lights_, depth, is_oren_nayar, roughness);
		}
		pixel_color /= samples_per_pixel;
	}
	pixel_color *= scale;

	// Surface acne
	pixel_color = Color(std::sqrt(pixel_color.x()), std::sqrt(pixel_color.y()), std::sqrt(pixel_color.z()));

	const auto r = Clamp(pixel_color.x(), 0.0, 0.999);
	const auto g = Clamp(pixel_color.y(), 0.0, 0.999);
	const auto b = Clamp(pixel_color.z(), 0.0, 0.999);

	image_[(j * image_width_ + i) * 3] = static_cast<int>(256 * r);
	image_[(j * image_width_ + i) * 3 + 1] = static_cast<int>(256 * g);
	image_[(j * image_width_ + i) * 3 + 2] = static_cast<int>(256 * b);
}

Color PathTracer::RayColorImportanceSampling(const Ray& ray, const Color& background,
                                             const std::shared_ptr<HittableList>& world,
                                             const std::shared_ptr<Hittable>& lights, const int depth,
                                             const bool is_oren_nayar,
                                             const double roughness, const int samples_per_pixel) const
{
	if (depth <= 0)
		return {0, 0, 0};

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

	Color accumulated_color = {0, 0, 0};
	for (int i = 0; i < samples_per_pixel; ++i)
	{
		if (s_rec.is_specular)
		{
			accumulated_color += s_rec.attenuation * RayColorImportanceSampling(
				s_rec.specular_ray, background, world, lights,
				depth - 1, is_oren_nayar, roughness,
				samples_per_pixel);
		}
		else
		{
			const auto light_ptr = std::make_shared<HittablePdf>(lights, rec.point);
			const MixturePdf mixture_pdf(light_ptr, s_rec.pdf);
			const auto scattered = Ray(rec.point, mixture_pdf.Generate());
			const auto pdf = mixture_pdf.Value(scattered.Direction());

			accumulated_color += emitted
				+ s_rec.attenuation * rec.material->ScatteringPdf(ray, rec, scattered)
				* RayColorImportanceSampling(scattered, background, world, lights, depth - 1, is_oren_nayar, roughness,
				                             samples_per_pixel) / pdf;
		}
	}

	return accumulated_color / samples_per_pixel;
}

Color PathTracer::RayColor(const Ray& ray, const Color& background, const std::shared_ptr<HittableList>& world,
                           const std::shared_ptr<Hittable>& lights, int depth, const bool is_oren_nayar,
                           const double roughness) const
{
	Color accumulated_color = {0, 0, 0};
	Color attenuation = {1, 1, 1};
	Ray current_ray = ray;

	while (depth > 0)
	{
		HitRecord rec;

		if (!world->Hit(current_ray, 0.001, infinity, rec))
		{
			accumulated_color += attenuation * background;
			break;
		}

		ScatterRecord s_rec;
		Color emitted;

		if (rec.material != nullptr)
		{
			emitted = rec.material->Emitted(current_ray, rec, rec.point);

			if (!rec.material->Scatter(current_ray, rec, s_rec, is_oren_nayar, roughness))
			{
				accumulated_color += attenuation * emitted;
				break;
			}
		}

		if (s_rec.is_specular)
		{
			accumulated_color += attenuation * s_rec.attenuation * emitted;
			current_ray = s_rec.specular_ray;
		}
		else
		{
			const auto light_ptr = std::make_shared<HittablePdf>(lights, rec.point);
			const MixturePdf mixture_pdf(light_ptr, s_rec.pdf);

			const auto scattered = Ray(rec.point, mixture_pdf.Generate());
			const auto pdf = mixture_pdf.Value(scattered.Direction());

			accumulated_color += attenuation * emitted * rec.material->ScatteringPdf(current_ray, rec, scattered)
				/ pdf;

			attenuation = attenuation * s_rec.attenuation;
			current_ray = scattered;
		}

		// Russian Roulette
		const double p = std::max(attenuation.x(), std::max(attenuation.y(), attenuation.z()));
		if (RandomDouble() > p)
			break;

		attenuation /= p;
		depth--;
	}

	return accumulated_color;
}

void PathTracer::GenerateRandomImages(const int count, const bool parallel) const
{
	// generate clean data
	for (int counter = 1; counter <= count; counter++)
	{
		// clean scene and generate random spheres
		this->CleanScene();
		const auto random_int = RandomInt(4, 14);
		for (int i = 0; i < random_int; i++)
			this->AddRandomSphere();

		// render image
		if (parallel)
		{
#pragma omp parallel for schedule(dynamic)
			for (int i = this->image_height_; i >= 0; i--)
				for (int j = 0; j <= this->image_width_; j++)
					this->Render(i, j, 50, 30, false, 0.5, true, 4, false);
		}
		else
		{
			for (int i = this->image_height_; i >= 0; i--)
				for (int j = 0; j <= this->image_width_; j++)
					this->Render(i, j, 50, 30, false, 0.5, true, 4, false);
		}

		// save image - absolute path for now... (todo)
		std::string location = R"(C:\Users\wrath\Pictures\PathTracer\generated_images\clean)";
		std::string path = location + R"(\generated_image_)" + std::to_string(counter) + ".jpg";
		stbi_flip_vertically_on_write(true);
		stbi_write_png(path.c_str(), image_width_, image_height_, 3,
		               image_, image_width_ * 3);

		
		// render noisy image
		for (int i = this->image_height_; i >= 0; i--)
			for (int j = 0; j <= this->image_width_; j++)
				this->Render(i, j, 2, 8, false, 0.5, true, 2, true);

		// save image - absolute path for now... (todo)
		location = R"(C:\Users\wrath\Pictures\PathTracer\generated_images\noisy)";
		path = location + R"(\generated_image_)" + std::to_string(counter) + ".jpg";
		stbi_flip_vertically_on_write(true);
		stbi_write_png(path.c_str(), image_width_, image_height_, 3,
					   image_, image_width_ * 3);
	}
}

void PathTracer::AddCornellBoxToWorld() const
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


void PathTracer::AddRandomSphere() const
{
	world_->Add(std::make_shared<Sphere>(Point3(RandomInt(0, 500), RandomInt(0, 500), RandomInt(-100, 500)),
	                                     RandomInt(5, 100), GetRandomMaterial()));
}

std::shared_ptr<Material> PathTracer::GetRandomMaterial() const
{
	const double r = RandomDouble();
	if (r < 1.0 / 3.0)
		return std::make_shared<Glass>(RandomDouble(0.5, 1.5));
	if (r < 2.0 / 3.0)
		return std::make_shared<Metal>(Color(RandomDouble(), RandomDouble(), RandomDouble()), RandomDouble());
	return std::make_shared<Lambertian>(Color(RandomDouble(), RandomDouble(), RandomDouble()));
}

std::shared_ptr<Material> PathTracer::GetRandomLambertMaterial() const
{
	return std::make_shared<Lambertian>(Color(RandomDouble(), RandomDouble(), RandomDouble()));
}

void PathTracer::AddRandomSphereLight() const
{
	const int intensity = RandomInt(5, 20);
	const auto light = std::make_shared<DiffuseLight>(Color(intensity, intensity, intensity));
	const auto random_radius = RandomInt(5, 30);
	const auto random_point = Point3(RandomInt(0, 500), RandomInt(0, 500), RandomInt(-200, 500));
	world_->Add(std::make_shared<FlipFace>(std::make_shared<Sphere>(random_point, random_radius, light)));
	lights_->Add(std::make_shared<Sphere>(random_point, random_radius));
}

void PathTracer::CleanScene() const
{
	world_->Clear();
	lights_->Clear();
	AddCornellBoxToWorld();
}

std::vector<ScreenBox> PathTracer::GetSphereScreenBoxes() const
{
	return world_->GetSphereScreenBoxes(*camera_, image_width_, image_height_);
}

void PathTracer::TagClosestObject() const
{
	world_->TagClosestObject(*camera_);
}
