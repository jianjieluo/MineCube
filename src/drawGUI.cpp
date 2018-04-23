#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

void draw_GUI() {
	ImGui::Begin("GUI Window", &show_gui_window, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Gouraud Shading", NULL, &is_gouraud_mode)) {
				is_gouraud_mode = true;
				is_phong_mode = false;
			}
			if (ImGui::MenuItem("Phong Shading", NULL, &is_phong_mode)) {
				is_gouraud_mode = false;
				is_phong_mode = true;
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "ESC")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Text("You can change to Basic/Bonus mode in Menu");

	ImGui::NewLine();
	ImGui::Text("You can change the parameters down here");
	ImGui::Text("Hint: Press 'Q' to exit/enter FPS mode");

	ImGui::Separator();
	ImGui::SliderFloat("ambient", &ambientStrength, 0.0f, 5.0f, "%.1f");
	//ImGui::SliderFloat("diffuse", &diffuse, 0.0f, 5.0f, "%.1f");
	ImGui::SliderFloat("specular", &specularStrength, 0.1f, 5.0f, "%.1f");
	ImGui::SliderFloat("shininess", &shininess, 16, 256, "%.1f");
	ImGui::Separator();

	ImGui::Checkbox("Show Light", &show_light);
	ImGui::Checkbox("Light Move", &light_move);

	ImGui::NewLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}