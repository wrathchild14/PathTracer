#include "Application.h"

Application::Application()
{
	auto* image = new uint8_t[400 * 400 * 3];
	tracer_ = std::make_unique<PathTracer>(image, 1.0);
}

Application::Application(const int width, const double aspect_ratio)
{
	auto* image = new uint8_t[width * width * 3];
	tracer_ = std::make_unique<PathTracer>(image, aspect_ratio);
}

void Application::SetWidth(const int width) const
{
	tracer_->SetImageWidth(width);
}

uint8_t* Application::GetImage() const
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

void Application::RenderRowMp(const int j, const int samples_per_pixel, const int depth, const bool is_oren_nayar,
                              const double roughness, const bool focusing, const int focus_multiplier,
                              const bool importance_sampling) const
{
#pragma omp parallel for
	for (int i = 0; i < tracer_->GetImageWidth(); ++i)
	{
		tracer_->Render(i, j, samples_per_pixel, depth, is_oren_nayar, roughness, focusing, focus_multiplier,
		                importance_sampling);
	}
}

void Application::RenderRow(const int j, const int samples_per_pixel, const int depth, const bool is_oren_nayar,
                            const double roughness, const bool focusing, const int focus_multiplier,
                            const bool importance_sampling) const
{
	for (int i = 0; i < tracer_->GetImageWidth(); ++i)
	{
		tracer_->Render(i, j, samples_per_pixel, depth, is_oren_nayar, roughness, focusing, focus_multiplier,
		                importance_sampling);
	}
}

void Application::GenerateRandomImages(const int count, const bool parallel) const
{
	tracer_->GenerateRandomImages(count, parallel);
}

std::vector<ScreenBox> Application::GetLabels() const
{
	return tracer_->GetSphereScreenBoxes();
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

void Application::TagClosestObject() const
{
	tracer_->TagClosestObject();
}
