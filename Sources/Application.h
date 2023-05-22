#pragma once

#include "PathTracer.h"

class Application
{
public:
	Application();
	Application(int width, double aspect_ratio);
	void SetWidth(int width) const;

	void RenderRowMp(int j, int samples_per_pixel, int depth, bool is_oren_nayar, double roughness, bool
	                 focusing, int focus_multiplier, bool importance_sampling) const;
	void RenderRow(int j, int samples_per_pixel, int depth, bool is_oren_nayar, double roughness, bool
	               focusing,
	               int focus_multiplier, bool importance_sampling) const;
	uint8_t* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

	void GenerateRandomImages(int count, bool parallel) const;
	std::vector<ScreenBox> GetLabels() const;

	void AddRandomSphere() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;

	void TagClosestObject() const;
	void SetImage(uint8_t* new_image);

private:
	std::unique_ptr<PathTracer> tracer_;
};
