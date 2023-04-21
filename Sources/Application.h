#pragma once
#include "Primitives/HittableList.h"
#include "Utility/Camera.h"
#include "Utility/Ray.h"
#include "Materials/DiffuseLight.h"
#include "Primitives/Rectangle.h"

#include "Materials/Lambertian.h"
#include "Primitives/Sphere.h"

class Application
{
public:
	Application(int width, double aspect_ratio);
	~Application();

	void Render(const int j, const int samples_per_pixel) const;
	unsigned char* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

private:
	static Color RayColor(const Ray& ray, const Color& background, const Hittable& world, std::shared_ptr<Hittable>& lights, const int depth);
	static double HitSphere(const Point3& center, double radius, const Ray& r);

	// Cornell Box example with 2 spheres
	HittableList GetCBExample() const;

	int width_{};
	int height_{};
	HittableList world_;
	Camera* camera_;
	int max_depth_;

	unsigned char* image_{};
	double aspect_ratio_{};
	Color background_ = Color(0, 0, 0);
};
