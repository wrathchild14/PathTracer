#pragma once

#include "PathTracer.h"

class Application
{
public:
	Application();
	Application(int width, double aspect_ratio);
	void SetWidth(int width) const;

	void RenderRowMp(const int j, const int samples_per_pixel, const int depth, const bool is_oren_nayar, const double roughness, const bool
	               focusing, const bool importance_sampling) const;
	void RenderRow(int j, int samples_per_pixel, int depth, bool is_oren_nayar, double roughness, bool focusing,
	               bool importance_sampling) const;
	uint8_t* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

	void GenerateRandomImages(int count) const;
	std::vector<ScreenBox> GetLabels() const;

	void AddRandomSphere() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;

	void TagClosestObject() const;

private:
	std::unique_ptr<PathTracer> tracer_;
};
