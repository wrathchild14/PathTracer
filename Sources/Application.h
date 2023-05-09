#pragma once
#include "Utility/Camera.h"
#include "Utility/Ray.h"

#include "Primitives/HittableList.h"
#include "Primitives/Rectangle.h"
#include "Primitives/Sphere.h"
#include "Primitives/FlipFace.h"

#include "Materials/Lambertian.h"
#include "Materials/DiffuseLight.h"
#include "Materials/Glass.h"
#include "Materials/Metal.h"

class Application
{
public:
	Application();
	Application(const int width, const double aspect_ratio);
	void SetWidth(int width);
	~Application();

	void Render(int j, int samples_per_pixel, int depth, bool is_russian_roulette, bool is_oren_nayar,
	            bool roughness) const;
	unsigned char* GetImage() const;

	int GetImageWidth() const;
	int GetImageHeight() const;

	void AddRandomSphere() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;

private:
	static Color RayColor(const Ray& ray, const Color& background, const std::shared_ptr<HittableList>& world,
	                      const std::shared_ptr<Hittable>& lights, int depth, bool is_oren_nayar, double roughness);
	static double HitSphere(const Point3& center, double radius, const Ray& r);

	void AddCornellBoxToWorld() const;
	std::shared_ptr<Material> RandomMaterial() const;

	int width_{};
	int height_{};
	std::shared_ptr<HittableList> lights_;
	std::shared_ptr<HittableList> world_;
	Camera* camera_;

	unsigned char* image_{};
	double aspect_ratio_{};
	Color background_ = Color(0.624, 1, 0.996);
};
