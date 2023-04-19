#include "Application.h"


Application::Application(const int width, const int height)
{
	width_ = width;
	height_ = height;
}

void Application::Render(const std::shared_ptr<char[]>& image) const
{
	for (int j = 0; j < width_; j++)
	{
		for (int i = 0; i < height_; i++)
		{
			auto r = static_cast<double>(i) / (width_ - 1);
			auto g = static_cast<double>(j) / (height_ - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			image[(i * width_ + j) * 3] = ir;
			image[(i * width_ + j) * 3 + 1] = ig;
			image[(i * width_ + j) * 3 + 2] = ib;
		}
	}
}
