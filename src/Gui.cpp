#include "Gui.hpp"

Gui::Gui(GLFWwindow* theWindow) {
	// ImGui initialization and bindings
	window = theWindow;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
}

void Gui::createNewFrame() {
	ImGui_ImplGlfwGL3_NewFrame();
}

void Gui::draw() {
	ImGui::Begin("GUI Window", &showGuiWindow, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			ImGui::MenuItem("An example item", NULL, &menuItem);
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "ESC")) glfwSetWindowShouldClose(window, true);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::NewLine();
	ImGui::Text("Hint: Press 'Q' to exit/enter FPS mode");

	ImGui::NewLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Gui::render() {
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::clear() {
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	window = nullptr;
}