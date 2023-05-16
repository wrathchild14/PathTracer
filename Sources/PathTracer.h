#pragma once
#include <string>

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

#include "stb_image_write.h"

class PathTracer
{
public:
	PathTracer() = default;
	PathTracer(unsigned char* image, double aspect_ratio);

	void SetImageWidth(int width);
	unsigned char* GetImage() const;
	int GetImageWidth() const;
	int GetImageHeight() const;
	int GetFocusAmountInLabels(int i, int j) const;

	void Render(const int i, const int j, const int samples_per_pixel, const int depth, const bool is_russian_roulette, const bool
	            is_oren_nayar,
	            const double roughness) const;
	double HitSphere(const Point3& center, double radius, const Ray& r) const;
	Color RayColor(const Ray& ray, const Color& background, const std::shared_ptr<HittableList>& world,
	               const std::shared_ptr<Hittable>& lights, const int depth, const bool is_oren_nayar, const double roughness, const int
	               samples_per_pixel) const;

	void GenerateRandomImages(int count) const;
	void AddCornellBoxToWorld() const;
	void AddRandomSphere() const;
	std::shared_ptr<Material> GetRandomMaterial() const;
	std::shared_ptr<Material> GetRandomLambertMaterial() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;
	std::vector<ScreenBox> GetSphereScreenBoxes() const;

	void SetFocusingAmount(int amount);
	void TagClosestObject() const;

private:
	unsigned char* image_{};
	Camera* camera_{};
	std::shared_ptr<HittableList> world_;
	std::shared_ptr<HittableList> lights_;
	int image_width_{};
	int image_height_{};

	Color background_ = Color(0.624, 1, 0.996);
	int focused_samples_per_pixel_ = 100;
};
