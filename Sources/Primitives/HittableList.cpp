#include "HittableList.h"

bool HittableList::Hit(const Ray& ray, const double t_min, const double t_max, HitRecord& rec) const
{
	HitRecord temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects_)
	{
		if (object->Hit(ray, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

double HittableList::PdfValue(const Point3& o, const Vec3& v) const
{
	const auto weight = 1.0 / objects_.size();
	auto sum = 0.0;

	for (const auto& object : objects_)
		sum += weight * object->PdfValue(o, v);

	return sum;
}

Vec3 HittableList::Random(const Vec3& o) const
{
	const auto int_size = static_cast<int>(objects_.size());
	return objects_[RandomInt(0, int_size - 1)]->Random(o);
}

std::vector<Label> HittableList::GetLabels(const Camera& camera, const int& width, const int& height) const
{
	for (const auto& object : objects_)
	{
		std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(object);
		if (sphere)
		{
			const auto sphere_center = sphere->GetCenter();

			const auto c = UnitVector(sphere_center - camera.GetPosition());
			const auto theta = acos(Dot(c, UnitVector(camera.GetDirection())));
			const auto d = sphere->GetRadius() / sin(theta);
			const auto h = 2 * d * tan(camera.GetFov() / 2);
			const auto w = h * camera.GetAspectRatio();

			Vec3 up = -UnitVector(camera.GetUp());
			Vec3 right = UnitVector(Cross(up, camera.GetDirection()));
			const auto ul = camera.GetPosition() + d * camera.GetDirection() - (h / 2.0) * up - (w / 2.0) * right;
			const auto lr = ul + h * up + w * right;

			const auto ul_normalized = Vec3((2.0 * ul.x() / width) - 1.0, (2.0 * ul.y() / height) - 1.0, 0);
			const auto lr_normalized = Vec3((2.0 * lr.x() / width) - 1.0, (2.0 * lr.y() / height) - 1.0, 0);
			
			// Assume width and height are the dimensions of the screen in pixels
			const auto x_ul_ndc = (2 * (ul_normalized.x() / width)) - 1;
			const auto y_ul_ndc = 1 - (2 * (ul_normalized.y() / height));
			const auto x_lr_ndc = (2 * (lr_normalized.x() / width)) - 1;
			const auto y_lr_ndc = 1 - (2 * (lr_normalized.y() / height));

			// Assume screen_width and screen_height are the dimensions of the screen in pixels
			const auto ul_x_screen = (x_ul_ndc + 1) * w / 2;
			const auto ul_y_screen = (y_ul_ndc + 1) * h / 2;
			const auto lr_x_screen = (x_lr_ndc + 1) * w / 2;
			const auto lr_y_screen = (y_lr_ndc + 1) * h / 2;

			std::cout << "Sphere with center on screen " << sphere_center << " is visible on the screen with dimensions "
					  << w << "x" << h << " pixels.\n";
			std::cout << "Upper left corner: " << ul_x_screen << " " << ul_y_screen << "\n";
			std::cout << "Lower right corner: " << lr_x_screen << " " << lr_y_screen << "\n";
		}
	}
	return {};
}
