#pragma once
#include <memory>
#include "Vec3.h"
#include "Ray.h"

class Application
{
public:
	Application(int width, double aspect_ratio);
	Application(int width, int height);

	void Render() const;
	unsigned char* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

private:
	static color RayColor(const Ray& r);

	int width_{};
	int height_{};

	unsigned char* image_{};
	double aspect_ratio_{};
};
