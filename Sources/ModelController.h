#pragma once

#include <cstdint>
#include <iostream>

#include <onnxruntime_cxx_api.h>

class ModelController
{
public:
	ModelController();
	void LoadModel(const uint8_t* image_data);
	void Run();
	uint8_t* GetResults();

private:
	Ort::Session session_;
	Ort::Value input_tensor_;
	Ort::Value output_tensor_;
	std::vector<const char*> input_names_;
	std::vector<const char*> output_names_;
	int64_t input_elements_;
	std::vector<float> input_;
	std::vector<float> output_;
	int channels;
	int width;
	int height;
};
