#include "HittableList.h"
#include <glm/gtc/matrix_transform.hpp>

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

std::vector<ScreenBox> HittableList::GetSphereScreenBoxes(const Camera& camera, const int& width, const int& height) const
{
	std::vector<ScreenBox> labels;
	for (const auto& object : objects_)
	{
		std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(object);
		if (sphere)
		{
			const auto camera_pos = camera.GetPosition();
			const auto camera_target = camera.GetTarget();
			const auto camera_up = camera.GetUp();
			const glm::mat4 view_matrix = lookAt(
				glm::vec3(camera_pos.x(), camera_pos.y(), camera_pos.z()),
				glm::vec3(camera_target.x(), camera_target.y(), camera_target.z()),
				glm::vec3(camera_up.x(), camera_up.y(), camera_up.z()));

			const double sphere_radius = sphere->GetRadius();
			const auto sphere_position = sphere->GetCenter();
			const auto view_position = glm::vec3(view_matrix * glm::vec4(sphere_position.x(),
			                                                             sphere_position.y(),
			                                                             sphere_position.z(), 1.0));
			const glm::mat4 projection_matrix = glm::perspective(glm::radians(camera.GetFov()),
			                                                     camera.GetAspectRatio(), 0.1, 100.0);
			const glm::vec4 clip_position = projection_matrix * glm::vec4(view_position, 1.0);
			const glm::vec3 ndc_position = glm::vec3(clip_position) / clip_position.w;

			const double half_fov_tan = tan(glm::radians(camera.GetFov() / 2.0));
			const double half_width = sphere_radius * static_cast<double>(ndc_position.z) * half_fov_tan * camera.
				GetAspectRatio()+ 15;
			const double half_height = sphere_radius * static_cast<double>(ndc_position.z) * half_fov_tan + 15;

			const double x = (static_cast<double>(ndc_position.x) + 1.0) * 0.5 * width;
			const double y = (1.0 - static_cast<double>(ndc_position.y)) * 0.5 * height;

			// const double x_min = x - half_width;
			// const double x_max = x + half_width;
			// const double y_min = y - half_height;
			// const double y_max = y + half_height;
			// auto top_left = Vec3(x_min, y_min, 0);
			// auto bottom_right = Vec3(x_max, y_max, 0);

			ScreenBox label{
				1, x, y, half_width * 2.0, half_height * 2.0
			};
			labels.push_back(label);
		}
	}
	return labels;
}
