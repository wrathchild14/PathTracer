#include "ModelController.h"

#include <algorithm>


ModelController::ModelController()
	: session_(nullptr), input_tensor_(nullptr), output_tensor_(nullptr), input_elements_(0)
{
	constexpr int64_t channels = 3;
	constexpr int64_t width = 400;
	constexpr int64_t height = 400;
	input_elements_ = channels * height * width;

	input_.resize(input_elements_);
}

void ModelController::LoadModel(const uint8_t* image_data)
{
	Ort::Env env;
	// Ort::Session session(nullptr);

	// note this works with loading a picture locally (will need to send data here)
	// const std::string image_file = "C:\\Git\\DenoisingProject\\image.jpg";
	const auto model_path = L"C:\\Git\\Masters\\nrg\\PathTracer\\Sources\\model_unet.onnx";

	Ort::SessionOptions ort_session_options;

	OrtCUDAProviderOptions options;
	options.device_id = 0;
	OrtSessionOptionsAppendExecutionProvider_CUDA(ort_session_options, options.device_id);

	session_ = Ort::Session(env, model_path, ort_session_options);


	for (int i = 0; i < input_elements_; i++)
	{
		// std::cout <<  image_data[i] << " ";
		const float normalized_value = (static_cast<float>(image_data[i]) / 127.5f) - 1.0f;
		input_[i] = normalized_value;
	}


	const std::array<int64_t, 4> input_shape = {1, 3, 400, 400};
	const std::array<int64_t, 4> output_shape = {1, 3, 400, 400};

	std::vector<float> output(input_elements_);

	auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
	
	input_tensor_ = Ort::Value::CreateTensor<float>(memory_info, input_.data(), input_.size(), input_shape.data(),
													input_shape.size());
	output_tensor_ = Ort::Value::CreateTensor<float>(memory_info, output.data(), output.size(), output_shape.data(),
													 output_shape.size());

	Ort::AllocatorWithDefaultOptions ort_alloc;
	Ort::AllocatedStringPtr input_name = session_.GetInputNameAllocated(0, ort_alloc);
	Ort::AllocatedStringPtr output_name = session_.GetOutputNameAllocated(0, ort_alloc);
	input_names_ = {input_name.get()};
	output_names_ = {output_name.get()};
	input_name.release();
	output_name.release();
}

void ModelController::Run()
{
	try
	{
		const Ort::RunOptions run_options;
		session_.Run(run_options, input_names_.data(), &input_tensor_, 1, output_names_.data(), &output_tensor_, 1);
	}
	catch (Ort::Exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

uint8_t* ModelController::GetResults()
{
	// std::vector<float> output_data(output_tensor_.GetTensorMutableData<float>(),
	//                                output_tensor_.GetTensorMutableData<float>() + output_tensor_.
	//                                GetTensorTypeAndShapeInfo().GetElementCount());
	const float* output_data_ptr = output_tensor_.GetTensorMutableData<float>();
	size_t output_data_size = output_tensor_.GetTensorTypeAndShapeInfo().GetElementCount();
	std::vector<float> output_data(output_data_ptr, output_data_ptr + output_data_size);
	
	// Scale the float values to the range of 0-255
	float min_value = *std::min_element(output_data.begin(), output_data.end());
	float max_value = *std::max_element(output_data.begin(), output_data.end());
	float range = max_value - min_value;

	std::vector<uint8_t> image_data(output_data.size());
	for (size_t i = 0; i < output_data.size(); ++i)
	{
		float scaled_value = (output_data[i] - min_value) / range * 255.0f;
		image_data[i] = static_cast<uint8_t>(scaled_value);
	}

	// You can now use the image data for further processing or return it from the function
	// Remember to manage memory appropriately, such as allocating memory for the image data using 'new' or 'malloc' and returning it to the caller. Make sure to free the memory when it's no longer needed.

	// Example: Returning the image data as a dynamically allocated array
	auto result = new uint8_t[image_data.size()];
	std::copy(image_data.begin(), image_data.end(), result);
	return result;
}