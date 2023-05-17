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
	bool IsInScreenBoxes(int i, int j) const;

	void Render(const int i, const int j, int samples_per_pixel, const int depth, const bool is_oren_nayar,
	            const double roughness, const bool focusing, const bool importance_sampling) const;
	Color RayColorImportanceSampling(const Ray& ray, const Color& background,
	                                 const std::shared_ptr<HittableList>& world,
	                                 const std::shared_ptr<Hittable>& lights, int depth, bool is_oren_nayar,
	                                 double roughness, int samples_per_pixel) const;
	Color RayColor(const Ray& ray, const Color& background, const std::shared_ptr<HittableList>& world,
	               const std::shared_ptr<Hittable>& lights, int depth, bool is_oren_nayar, double roughness) const;

	void GenerateRandomImages(const int count, const bool parallel) const;
	void AddCornellBoxToWorld() const;
	void AddRandomSphere() const;
	std::shared_ptr<Material> GetRandomMaterial() const;
	std::shared_ptr<Material> GetRandomLambertMaterial() const;
	void AddRandomSphereLight() const;
	void CleanScene() const;
	std::vector<ScreenBox> GetSphereScreenBoxes() const;

	void TagClosestObject() const;

private:
	uint8_t* image_;
	Camera* camera_{};
	std::shared_ptr<HittableList> world_;
	std::shared_ptr<HittableList> lights_;
	int image_width_{};
	int image_height_{};

	Color background_ = Color(0.624, 1, 0.996);
};
