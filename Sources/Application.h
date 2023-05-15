#pragma once

#include "PathTracer.h"

class Application
{
public:
	Application();
	Application(int width, double aspect_ratio);
	void SetWidth(int width) const;

	void RenderRow(int j, int samples_per_pixel, int depth, bool is_russian_roulette,
	            bool is_oren_nayar,
	            double roughness) const;
	unsigned char* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

	void GenerateRandomImages(int count) const;
	std::vector<Label> GetLabels() const;

	void AddRandomSphere() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;

private:
	std::unique_ptr<PathTracer> tracer_;
};
