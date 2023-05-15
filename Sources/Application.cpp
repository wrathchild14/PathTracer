#include "Application.h"

Application::Application()
{
	auto* image = new std::uint8_t[400 * 400 * 3];
	tracer_ = std::make_unique<PathTracer>(image, 1.0);
}

Application::Application(const int width, const double aspect_ratio)
{
	auto* image = new std::uint8_t[width * width * 3];
	tracer_ = std::make_unique<PathTracer>(image, aspect_ratio);
}

void Application::SetWidth(const int width) const
{
	tracer_->SetImageWidth(width);
}

std::uint8_t* Application::GetImage() const
{
	return tracer_->GetImage();
}

int Application::GetImageWidth() const
{
	return tracer_->GetImageWidth();
}

int Application::GetImageHeight() const
{
	return tracer_->GetImageHeight();
}

void Application::RenderRow(const int j, const int samples_per_pixel, const int depth,
                         const bool is_russian_roulette, const bool is_oren_nayar, const double roughness) const
{
	for (int i = 0; i < tracer_->GetImageWidth(); ++i)
	{
		tracer_->Render(i, j, samples_per_pixel, depth, is_russian_roulette, is_oren_nayar, roughness);
	}
}

void Application::GenerateRandomImages(const int count) const
{
	tracer_->GenerateRandomImages(count);
}

std::vector<Label> Application::GetLabels() const
{
	return tracer_->GetSphereLabels();
}

void Application::AddRandomSphere() const
{
	tracer_->AddRandomSphere();
}

void Application::AddRandomSphereLight() const
{
	tracer_->AddRandomSphereLight();
}

void Application::CleanScene() const
{
	tracer_->CleanScene();
}
