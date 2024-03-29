#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tinyfiledialogs.h>
#include <omp.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

#include "Application.h"
// #include "ModelController.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	auto glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1400, 720, "Path tracer", nullptr, nullptr);
	if (window == nullptr)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync (can be on, for now)

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Settings
	// auto* model_controller = new ModelController();

	const auto application = new Application();
	application->SetWidth(400);
	int width = application->GetImageWidth();
	int height = application->GetImageHeight();
	int sample_depth = 8;
	int samples_per_pixel = 2;
	int row_counter = height - 1;
	int focus_multiplier = 2;

	bool multi_processing = false;
	bool should_image_render = false;
	bool focusing = false;
	bool importance_sampling = true;
	bool is_oren_nayar = false;

	float roughness = 0.5;
	int number_g_images = 5;
	double render_start_time;

	const auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
#ifdef __EMSCRIPTEN__
	// For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
	// You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
	io.IniFilename = nullptr;
	EMSCRIPTEN_MAINLOOP_BEGIN
#else
	while (!glfwWindowShouldClose(window))
#endif
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		ImGui::Begin("Properties", nullptr);
		ImGui::SliderInt("Sample depth", &sample_depth, 1, 60);
		ImGui::SliderInt("Samples per pixel", &samples_per_pixel, 1, 300);
		ImGui::Checkbox("Oren-Nayar", &is_oren_nayar);
		ImGui::SliderFloat("Oren-Nayar roughness", &roughness, 0.0, 1.0);

		if (ImGui::Button("Render"))
		{
			render_start_time = omp_get_wtime(); // Start the timer
			row_counter = height - 1;
			should_image_render = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Render (closest)"))
		{
			application->TagClosestObject();
			render_start_time = omp_get_wtime(); // Start the timer
			row_counter = height - 1;
			should_image_render = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop render"))
		{
			should_image_render = false;
		}
		ImGui::SameLine();
		// if (ImGui::Button("Magic"))
		// {
		// 	const auto image = application->GetImage();
		// 	model_controller->RunModel(image, "unet", width, height, 3);
		// 	const auto new_image = model_controller->GetResults();
		// 	if (new_image != nullptr)
		// 	{
		// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		// 		             GL_UNSIGNED_BYTE, new_image);
		// 		application->SetImage(new_image);
		// 	}
		// }
		// ImGui::SameLine();
		// if (ImGui::Button("General magic"))
		// {
		// 	const auto image = application->GetImage();
		// 	model_controller->RunModel(image, "general_unet", width, height, 3);
		// 	const auto new_image = model_controller->GetResults();
		// 	if (new_image != nullptr)
		// 	{
		// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		// 		             GL_UNSIGNED_BYTE, new_image);
		// 		application->SetImage(new_image);
		// 	}
		// }

		ImGui::Checkbox("MP", &multi_processing);
		ImGui::SameLine();
		ImGui::Checkbox("Focusing", &focusing);
		ImGui::SameLine();
		ImGui::Checkbox("Importance sampling", &importance_sampling);

		ImGui::SliderInt("Focus multiplier", &focus_multiplier, 1, 25);

		ImGui::Text("Image settings");
		if (ImGui::Button("200 width"))
		{
			should_image_render = false;
			application->SetWidth(200);
			width = application->GetImageWidth();
			height = application->GetImageHeight();
		}
		ImGui::SameLine();
		if (ImGui::Button("400 width"))
		{
			should_image_render = false;
			application->SetWidth(400);
			width = application->GetImageWidth();
			height = application->GetImageHeight();
		}
		ImGui::SameLine();
		if (ImGui::Button("600 width"))
		{
			should_image_render = false;
			application->SetWidth(600);
			width = application->GetImageWidth();
			height = application->GetImageHeight();
		}
		ImGui::SameLine();
		if (ImGui::Button("800 width"))
		{
			should_image_render = false;
			application->SetWidth(800);
			width = application->GetImageWidth();
			height = application->GetImageHeight();
		}

		if (ImGui::Button("Save Image"))
		{
			std::string file_path = tinyfd_saveFileDialog("Save Image", "image.jpg", 0, nullptr, nullptr);
			if (!file_path.empty())
			{
				stbi_flip_vertically_on_write(true);
				stbi_write_png(file_path.c_str(), application->GetImageWidth(), application->GetImageHeight(), 3,
				               application->GetImage(), application->GetImageWidth() * 3);
			}
		}

		ImGui::Text("Scene settings");
		if (ImGui::Button("Add random sphere"))
			application->AddRandomSphere();
		ImGui::SameLine();
		if (ImGui::Button("Add random sphere light"))
			application->AddRandomSphereLight();
		if (ImGui::Button("Clear scene"))
			application->CleanScene();

		ImGui::Text("Image generation settings");
		ImGui::InputInt("#images", &number_g_images);
		if (ImGui::Button("Generate"))
			application->GenerateRandomImages(number_g_images, multi_processing);

		if (ImGui::Button("Tag closest"))
		{
			application->TagClosestObject();
		}


		ImGui::End();

		if (should_image_render)
		{
			if (multi_processing)
			{
#pragma omp parallel for
				for (int i = row_counter; i >= 0; i--)
				{
					application->RenderRowMp(i, samples_per_pixel, sample_depth, is_oren_nayar, roughness, focusing,
					                         focus_multiplier, importance_sampling
					);
				}
				const auto image = application->GetImage();
				if (image != nullptr)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
				should_image_render = false;
				const double end_time = omp_get_wtime(); // Stop the timer
				const double elapsed_time = end_time - render_start_time;
				printf("Rendered image: %f seconds\n", elapsed_time);
			}
			else
			{
				if (row_counter >= 0)
				{
					application->RenderRow(row_counter, samples_per_pixel, sample_depth, is_oren_nayar, roughness,
					                       focusing, focus_multiplier, importance_sampling);
					if (row_counter % 10 == 0)
					{
						const auto image = application->GetImage();
						if (image != nullptr)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
							             GL_UNSIGNED_BYTE, image);
						}
					}
				}
				else
				{
					should_image_render = false;
					const double end_time = omp_get_wtime(); // Stop the timer
					const double elapsed_time = end_time - render_start_time;
					printf("Rendered image: %f seconds\n", elapsed_time);
				}
				row_counter--;
			}
		}

		ImGui::Begin("Render window", nullptr);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		const auto viewport_width = ImGui::GetContentRegionAvail().x;
		const auto viewport_height = ImGui::GetContentRegionAvail().y;
		const float scale_factor_x = static_cast<float>(width) / viewport_width;
		const float scale_factor_y = static_cast<float>(height) / viewport_height;
		const ImVec2 image_size(viewport_width, viewport_width * (static_cast<float>(height) / width));
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), image_size,
		             ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::IsItemClicked())
		{
			const auto rec_min = ImGui::GetItemRectMin();
			const auto mouse_pos = ImGui::GetMousePos();
			const auto click_pos = ImVec2(mouse_pos.x - rec_min.x, mouse_pos.y - rec_min.y);

			const auto clicked_pixel_x = static_cast<int>(click_pos.x * scale_factor_x);
			const auto clicked_pixel_y = static_cast<int>(click_pos.y * scale_factor_y);

			if (application->TagObject(clicked_pixel_x, clicked_pixel_y))
				printf("Tagged object at: (%d, %d)\n", clicked_pixel_x, clicked_pixel_y);
		}

		ImGui::End();


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
		             clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_MAINLOOP_END;
#endif

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return
		0;
}
