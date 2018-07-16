#include "Gui.hpp"
#include <iostream>
#include "string.h"

Gui::Gui(GLFWwindow* theWindow, Text* _text) {
	// ImGui initialization and bindings
	window = theWindow;
	ImGui::CreateContext();

	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;

	colorBar = true;
	editBar = true;
	workBar = true;
	saveWindow = false;
	camera = Camera::getInstance();

	latest_color = ImVec4(cubes_color[0], cubes_color[1], cubes_color[2], cubes_color[3]);

	allowCloth = true;
	isFullNeed = false;
	specialEffect = 0;
	text = _text;
}

void Gui::createNewFrame() {
	ImGui_ImplGlfwGL3_NewFrame();
}

bool Gui::isClothAllow() {
	return allowCloth;
}

// Demonstrate creating a fullscreen menu bar and populating it.
void Gui::showAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		//if (ImGui::BeginMenu("File"))
		//{
		//	if (ImGui::MenuItem("New")) 
		//	{
		//		// TODO
		//	}
		//	if (ImGui::MenuItem("Open", "Ctrl+O"))
		//	{
		//		// TODO
		//	}
		//	if (ImGui::BeginMenu("Open Recent"))
		//	{
		//		// TODO
		//		ImGui::EndMenu();
		//	}
		//	if (ImGui::MenuItem("Save", "Ctrl+S"))
		//	{
		//		// TODO
		//	}
		//	if (ImGui::MenuItem("Save As.."))
		//	{
		//		// TODO
		//	}
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("Quit", "Alt+F4")) 
		//	{
		//		// TODO
		//	}
		//	ImGui::EndMenu();
		//}

		if (ImGui::BeginMenu("Edit"))
		{
			if (operationManager.getIndexOfHistory() <= 0) {
				if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
			}
			else {
				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{
					operationManager.undo();
				}
			}

			if (operationManager.getIndexOfHistory() == operationManager.str_history.size()) {
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
			}
			else {
				if (ImGui::MenuItem("Redo", "CTRL+Y"))
				{
					operationManager.cancle_undo();
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Color Bar", NULL, &colorBar));

			if (ImGui::MenuItem("Edit Bar", NULL, &editBar));
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Text("Hint: Press 'V' to switch Moving Mode");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Gui::showEditBar() {
	ImGui::Begin("Edit Bar", &editBar, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(260, screenHeight));
		ImGui::SetWindowPos(ImVec2(screenWidth - 260, 18));
	}


	ImGui::Text("Mode: ");
	{	// add button
		ImGui::PushID(0);
		if (mode == CREATE_MODE) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
		}
		if (ImGui::Button("  Add  ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
			setMode_add();
		};
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	ImGui::SameLine();
	{	// erase button
		ImGui::PushID(1);
		if (mode == ERASE_MODE) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
		}
		if (ImGui::Button(" Erase ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
			setMode_erase();
		};
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	ImGui::SameLine();
	{	// paint button
		ImGui::PushID(2);
		if (mode == PAINT_MODE) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
		}
		if (ImGui::Button(" Paint ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
			setMode_print();
		};
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	ImGui::Text("Tool:");
	{
		if (ImGui::Button("Full", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
			isFullNeed = true;
		}

		ImGui::SameLine();

		if (allowCloth) {
			if (ImGui::Button("Close Cloth", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
				allowCloth = false;
			}
		}
		else {
			if (ImGui::Button("Open Cloth", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
				allowCloth = true;
			}
		}
	}

	ImGui::Text("Special Effect:");
	{
		{	// normal button
			ImGui::PushID(0);
			if (specialEffect == 0) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
			}
			if (ImGui::Button("Normal", ImVec2(ImGui::GetWindowWidth() * 0.9 + 8.0f, 20.0f))) {
				specialEffect = 0;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		{	// Inversion button
			ImGui::PushID(1);
			if (specialEffect == 1) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
			}
			if (ImGui::Button("Inversion", ImVec2(ImGui::GetWindowWidth() * 0.45, 20.0f))) {
				specialEffect = 1;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		ImGui::SameLine();

		{	// Grayscale button
			ImGui::PushID(2);
			if (specialEffect == 2) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
			}
			if (ImGui::Button("Grayscale", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
				specialEffect = 2;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		{	// Grayscale button
			ImGui::PushID(3);
			if (specialEffect == 3) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
			}
			if (ImGui::Button("Sharpen", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
				specialEffect = 3;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		ImGui::SameLine();

		{	// Grayscale button
			ImGui::PushID(4);
			if (specialEffect == 4) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
			}
			if (ImGui::Button("Blur", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
				specialEffect = 4;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
	}

	ImGui::Text("");
	ImGui::Text("History:");
	if (screenHeight >= 600) {
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth()* 0.95, screenHeight > 700? ImGui::GetWindowHeight() * 0.15 : ImGui::GetWindowHeight() * 0.1), true);

		static int his_selected = operationManager.getIndexOfHistory() - 1;
		for (int i = 0; i < operationManager.getIndexOfHistory(); i++)
		{
			char buf[32];
			sprintf(buf, operationManager.str_history[i].c_str());
			if (ImGui::Selectable(buf, his_selected == i))
				his_selected = i;
		}

		ImGui::EndChild();
	}

	{	//undo button
		ImGui::PushID(0);
		if (operationManager.getIndexOfHistory() <= 0) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(120, 120, 120, 120));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(120,120,120, 120));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(120, 120, 120, 120));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
		}
		if (ImGui::Button("Undo", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
			operationManager.undo();
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	{	//redo button
		ImGui::SameLine();
		ImGui::PushID(0);
		if (operationManager.getIndexOfHistory() == operationManager.str_history.size()) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(120, 120, 120, 120));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(120, 120, 120, 120));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(120, 120, 120, 120));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4 / 7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4 / 7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4 / 7.0f, 0.8f, 0.8f));
		}
		if (ImGui::Button("Redo", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {
			operationManager.cancle_undo();
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
	

	ImGui::Text("");
	ImGui::Text("Default:");
	if (screenHeight >= 450) {
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(ImGui::GetWindowWidth()* 0.95, ImGui::GetWindowHeight() * 0.2), true);

		static int def_selected = -1;
		vector<string> file_list = ptr_cubeManager->getModels();
		for (int i = 0; i < file_list.size(); i++)
		{
			char buf[32];
			sprintf(buf, "%s", file_list[i].c_str());
			if (ImGui::Selectable(buf, def_selected == i)) {
				ptr_cubeManager->load(buf);
				def_selected = i;
			}
		}
		ImGui::EndChild();
	}

	{	//Save button
		if (ImGui::Button("Save", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f)))
			ImGui::OpenPopup("Save");
		if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			saveWindow = true;
			ImGui::Text("File name:\n");
			static char buf[64] = "";
			ImGui::InputText("", buf, 64);
			ImGui::Text("\n");
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0))) {
				// TODO
				ptr_cubeManager->dump(buf);
				ImGui::CloseCurrentPopup();
				saveWindow = false;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
				ImGui::CloseCurrentPopup(); 
				saveWindow = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::SameLine();
	{	//Open button
		if (ImGui::Button("Open", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f)))
			ImGui::OpenPopup("Open");
		if (ImGui::BeginPopupModal("Open", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			saveWindow = true;
			ImGui::Text("File name:\n");
			static char buf[64] = "";
			ImGui::InputText("", buf, 64);
			ImGui::Text("\n");
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0))) {
				// TODO
				auto result = ptr_cubeManager->load(buf);
				// cout << result << endl;
				if (result == false) {
					strcpy(buf, "load fail");
				}
                if (result == true) {
                    ImGui::CloseCurrentPopup();
                }
				saveWindow = false;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
				ImGui::CloseCurrentPopup(); 
				saveWindow = false;
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Text("");
	ImGui::Text("Other Options:");
    // Change light attribute
    if (ImGui::CollapsingHeader("lighting options")) {
        ImGui::SliderFloat("lightPos.x", &lightPos.x, -10.0f, 10.0f, "X = %.1f");
        ImGui::SliderFloat("lightPos.y", &lightPos.y, -10.0f, 10.0f, "Y = %.1f");
        ImGui::SliderFloat("lightPos.z", &lightPos.z, -10.0f, 10.0f, "Z = %.1f");
    }

	ImGui::End();
}

void Gui::showColorBar() {
	ImGui::Begin("Color Bar", &colorBar, 0 | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(260, screenHeight));
		ImGui::SetWindowPos(ImVec2(0, 18));
	}


	ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoAlpha;

	ImGui::ColorPicker4("Color##4", (float*)&cubes_color, flags, NULL);

	if (latest_color.x != cubes_color[0] || latest_color.y != cubes_color[1] || 
			latest_color.z != cubes_color[2] || latest_color.w != cubes_color[3]) {
		if (ImGui::IsMouseReleased(0)) {
			addColor2His(ImVec4(cubes_color[0], cubes_color[1], cubes_color[2], cubes_color[3]));
		}
	}

	{	// color history
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth() * 0.95, 80), true);
		for (int i = colorHistory.size() - 1; i >= 0; i--)
		{
			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_Button, colorHistory[i]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHistory[i]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorHistory[i]);
			if (ImGui::Button("     ", ImVec2(ImGui::GetWindowWidth() * 0.15, 30.0f))) {
				cubes_color[0] = colorHistory[i].x;
				cubes_color[1] = colorHistory[i].y;
				cubes_color[2] = colorHistory[i].z;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
			if (i != colorHistory.size() - 5) ImGui::SameLine();
		}
		ImGui::EndChild();
	}

	{	// color list
		ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(ImGui::GetWindowWidth() * 0.95, screenHeight > 400 ? (screenHeight - 80 - 240) * 0.8 : screenHeight * 0.1), true);
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (j != 0) ImGui::SameLine();
				ImGui::PushID(i * 5 + j);
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f, 0.5f + j * 0.1f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f + 0.05f, 0.5f + j * 0.1f + 0.05f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f + 0.1f, 0.5f + j * 0.1f + 0.1f));
				if (ImGui::Button("     ", ImVec2(ImGui::GetWindowWidth() * 0.15, 30.0f))) {
					static ImVec4 saved_palette;
					ImGui::ColorConvertHSVtoRGB(i / 100.0f, 0.5f + j * 0.1f, 0.5f + j * 0.1f, saved_palette.x, saved_palette.y, saved_palette.z);
					cubes_color[0] = saved_palette.x;
					cubes_color[1] = saved_palette.y;
					cubes_color[2] = saved_palette.z;
					saved_palette.w = 1;
					// save the color
					addColor2His(saved_palette);
				}
				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}
		}
		ImGui::EndChild();
	}

	ImGui::End();
}

void Gui::showWorkBar(unsigned int textureColorbuffer) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::Begin("Work Bar", &workBar, 0 | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(screenWidth / 2 + style.WindowPadding.x * 2, screenHeight / 2 + style.WindowPadding.y * 2 + 24));
		ImGui::SetWindowPos(ImVec2(260, 18));
	}
	workBarPos[0] = ImGui::GetWindowPos().x;
	workBarPos[1] = ImGui::GetWindowPos().y;
	ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(screenWidth / 2, screenHeight / 2), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
	ImGui::End();
}

float* Gui::getWorkBarPos() {
	return workBarPos;
}


void Gui::draw(unsigned int textureColorbuffer) {
	showAppMainMenuBar();
	if (colorBar) showColorBar();
	if (editBar) showEditBar();
	if (workBar) showWorkBar(textureColorbuffer);
	if (isFirstDraw) isFirstDraw = false;
}

void Gui::draw() {
	showAppMainMenuBar();
	if (colorBar) showColorBar();
	if (editBar) showEditBar();
	if (isFirstDraw) isFirstDraw = false;
}

void Gui::render() {
	captureKeys();
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::clear() {
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	window = nullptr;
}

void Gui::autoRePos() {
	isFirstDraw = true;
}

void Gui::setMode_add() {
	mode = CREATE_MODE;
	text->push("ADD", glm::vec3(1.0f, 1.0f, 1.0f), 50);
}

void Gui::setMode_print() {
	mode = PAINT_MODE;
	text->push("PAINT", glm::vec3(1.0f, 1.0f, 1.0f), 50);
}

void Gui::setMode_erase() {
	mode = ERASE_MODE;
	text->push("ERASE", glm::vec3(1.0f, 1.0f, 1.0f), 50);
}

void Gui::captureKeys() {
	if (!saveWindow) {
		ImGuiIO& io = ImGui::GetIO();
		if (io.KeyCtrl) {
			//	ctrl + z
			if (ImGui::IsKeyReleased(GLFW_KEY_Z)) {
				operationManager.undo();
			}
			//	ctrl + y
			if (ImGui::IsKeyReleased(GLFW_KEY_Y)) {
				operationManager.cancle_undo();
			}
		}

		// release v
		if (ImGui::IsKeyReleased(GLFW_KEY_V)) {
			camera->isFpsMode = !camera->isFpsMode;
			camera->pause();
		}

		// ¹öÂÖ
		if (io.MouseWheel && io.MousePos.x > 260 && io.MousePos.x < screenWidth - 260) {
			camera->zoomInOrOut(io.MouseWheel);
		}

		if (io.KeysDown[GLFW_KEY_W]) {
			camera->moveCamera(FORWARD, deltaTime);
		}
		else if (io.KeysDown[GLFW_KEY_S]) {
			camera->moveCamera(BACKWARD, deltaTime);
		}

		if (io.KeysDown[GLFW_KEY_A]) {
			camera->moveCamera(LEFT, deltaTime);
		}
		else if (io.KeysDown[GLFW_KEY_D]) {
			camera->moveCamera(RIGHT, deltaTime);
		}
	}
}

void Gui::addColor2His(ImVec4 color) {
	if (latest_color.x != color.x || latest_color.y != color.y || latest_color.z != color.z) {
		if (colorHistory.size() < 10) {
			colorHistory.push_back(color);
		}
		else {
			for (auto i = 0; i < 9; i++) {
				colorHistory[i] = colorHistory[i + 1];
			}
			colorHistory[9] = color;
		}
		latest_color = color;
	}
}

bool Gui::isSaveWindowShow() {
	return saveWindow;
}


void Gui::setPtrCubeManager(CubeManager* ptr) {
	ptr_cubeManager = ptr;
}

int Gui::getSpecialEffect() {
	return specialEffect;
}

void Gui::updateSE(Shader* shader) {
	shader->setInt("specialEffect", specialEffect);
}