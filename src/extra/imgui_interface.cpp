#include "imgui_interface.hpp"

namespace Voyage {

void ImGuiInterface::init(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
}

void ImGuiInterface::sliderMat4(const char* label, glm::mat4 mat, float min, float max) {
	ImGui::SliderFloat4("", &mat[0][0], min, max);
	ImGui::SliderFloat4("", &mat[1][0], min, max);
	ImGui::SliderFloat4("", &mat[2][0], min, max);
	ImGui::SliderFloat4(label, &mat[3][0], min, max);
	ImGui::Separator();
}

void ImGuiInterface::startFrame() {
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void ImGuiInterface::startWindow(const char* name) { ImGui::Begin(name); }

void ImGuiInterface::endWindow() { ImGui::End(); }

void ImGuiInterface::endFrame() { ImGui::EndFrame(); }

void ImGuiInterface::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiInterface::dispose() {
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

}
