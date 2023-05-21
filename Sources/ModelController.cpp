﻿#include "ModelController.h"


ModelController::ModelController() : session_(nullptr), input_tensor_(nullptr), output_tensor_(nullptr)
{
	channels_ = 3;
	width_ = 400;
	height_ = 400;
	input_elements_ = channels_ * height_ * width_;

	input_.resize(input_elements_);
}

void ModelController::LoadModel(const uint8_t* image_data)
{
	const auto model_path = L"C:\\Git\\Masters\\nrg\\PathTracer\\models\\model_unet.onnx";

	const Ort::Env env;

	const Ort::SessionOptions ort_session_options;
	OrtCUDAProviderOptions options;
	options.device_id = 0;
	OrtSessionOptionsAppendExecutionProvider_CUDA(ort_session_options, options.device_id);

	session_ = Ort::Session(env, model_path, ort_session_options);

	input_.resize(input_elements_);
	output_.resize(input_elements_);
	
	for (int row = 0; row < height_; ++row)
	{
		for (int col = 0; col < width_; ++col)
		{
			for (int ch = 0; ch < channels_; ++ch)
			{
				const size_t index = (row * width_ + col) * channels_ + ch;
				input_[ch * width_ * height_ + row * width_ + col] = static_cast<float>(image_data[index]) / 127.5f - 1.0f;
			}
		}
	}

	const std::array<int64_t, 4> input_shape = {1, channels_, height_, width_};
	const std::array<int64_t, 4> output_shape = {1, channels_, height_, width_};

	const Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
	input_tensor_ = Ort::Value::CreateTensor<float>(memory_info, input_.data(), input_elements_,
	                                                input_shape.data(), input_shape.size());
	output_tensor_ = Ort::Value::CreateTensor<float>(memory_info, output_.data(), output_.size(),
	                                                 output_shape.data(), output_shape.size());

	Ort::AllocatorWithDefaultOptions ort_alloc;
	Ort::AllocatedStringPtr input_name = session_.GetInputNameAllocated(0, ort_alloc);
	Ort::AllocatedStringPtr output_name = session_.GetOutputNameAllocated(0, ort_alloc);
	input_names_ = {input_name.get()};
	output_names_ = {output_name.get()};
	input_name.release();
	output_name.release();

	try
	{
		const Ort::RunOptions run_options;
		session_.Run(run_options, input_names_.data(), &input_tensor_, 1, output_names_.data(), &output_tensor_, 1);
	}
	catch (Ort::Exception& e)
	{
		// std::cout << e.what() << std::endl;
	}
}

uint8_t* ModelController::GetResults() const
{
	const auto* output_data_ptr = output_tensor_.GetTensorData<float>();
	const size_t output_data_size = output_tensor_.GetTensorTypeAndShapeInfo().GetElementCount();
	const std::vector<float> output_data(output_data_ptr, output_data_ptr + output_data_size);

	std::vector<float> output_normalized(output_data.size());
	for (size_t i = 0; i < output_data.size(); ++i)
	{
		output_normalized[i] = (output_data[i] + 1.0) / 2.0;
	}

	const int height = 400;
	const int width = 400;
	std::vector<uint8_t> image_data(height * width * 3);

	for (size_t row = 0; row < height; ++row)
	{
		for (size_t col = 0; col < width; ++col)
		{
			for (size_t ch = 0; ch < 3; ++ch)
			{
				const size_t index = (row * width + col) * 3 + ch;
				const float value = output_normalized[ch * height * width + row * width + col];
				const float scaled_value = value * 255.0f;
				image_data[index] = static_cast<uint8_t>(scaled_value);
			}
		}
	}

	auto* result = new uint8_t[image_data.size()];
	std::copy(image_data.begin(), image_data.end(), result);
	return result;
}
