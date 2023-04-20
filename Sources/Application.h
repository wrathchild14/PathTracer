#pragma once
#include "Utility/Ray.h"
#include "Utility/Vec3.h"

class Application
{
public:
	Application(int width, double aspect_ratio);
	Application(int width, int height);
	~Application();

	void Render() const;
	unsigned char* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

private:
	static Color RayColor(const Ray& r);
	static double HitSphere(const Point3& center, double radius, const Ray& r);

	int width_{};
	int height_{};

	unsigned char* image_{};
	double aspect_ratio_{};
};
