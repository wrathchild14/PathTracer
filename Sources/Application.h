#pragma once
#include <memory>

class Application
{
public:
	Application(int width, int height);

	void Render(const std::shared_ptr<char[]>& image) const;

private:
	int width_;
	int height_;
};
