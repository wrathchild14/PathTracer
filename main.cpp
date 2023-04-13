#include "imgui.h"

int main()
{
    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Main loop
    while (true)
    {
        // Start a new frame
        ImGui::NewFrame();

        // Create UI elements here
        ImGui::ShowDemoWindow();

        // Render UI
        ImGui::Render();

        // Handle events
        /* handle events here */
    }

    // Clean up ImGui
    ImGui::DestroyContext();

    return 0;
}
