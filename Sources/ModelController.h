#pragma once

#include <cstdint>

#include <onnxruntime_cxx_api.h>

class ModelController
{
public:
	ModelController();
	void RunModel(const uint8_t* image_data, const std::string& model_name, const int width, const int height, const int channels);
	uint8_t* GetResults() const;

private:
	Ort::Session session_;
	Ort::Value input_tensor_;
	Ort::Value output_tensor_;
	std::vector<const char*> input_names_;
	std::vector<const char*> output_names_;
	int64_t input_elements_{};
	std::vector<float> input_;
	std::vector<float> output_;
	int channels_{};
	int width_{};
	int height_{};
};
