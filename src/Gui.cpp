#include "Gui.hpp"
#include <iostream>

Gui::Gui(GLFWwindow* theWindow) {
	// ImGui initialization and bindings
	window = theWindow;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;

	colorBar = true;
	editBar = true;
	workBar = true;
}

void Gui::createNewFrame() {
	ImGui_ImplGlfwGL3_NewFrame();
}

// Demonstrate creating a fullscreen menu bar and populating it.
void Gui::showAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				// TODO
			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				// TODO
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				// TODO
			}
			if (ImGui::MenuItem("Save As.."))
			{
				// TODO
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "Alt+F4")) 
			{
				// TODO
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) 
			{
				operationManager.undo();
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) 
			{
				// TODO
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) 
			{
				// TODO
			}
			if (ImGui::MenuItem("Copy", "CTRL+C")) 
			{
				// TODO
			}
			if (ImGui::MenuItem("Paste", "CTRL+V")) 
			{
				// TODO
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
			ImGui::Text("Hint: Press 'Z' to switch Edit Mode");
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

	switch (mode)
	{
	case ERASE_MODE:
		ImGui::Text("Mode: Erase");
		break;
	case CREATE_MODE:
		ImGui::Text("Mode: Add");
		break;
	case PAINT_MODE:
		ImGui::Text("Mode: Paint");
		break;
	default:
		break;
	}

	if (ImGui::Button("  Add  ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
		setMode_add();
	};
	ImGui::SameLine();
	if (ImGui::Button(" Erase ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
		setMode_erase();
	};
	ImGui::SameLine();
	if (ImGui::Button(" Paint ", ImVec2(ImGui::GetWindowWidth()* 0.29, 20.0f))) {
		setMode_print();
	};

	ImGui::Text("");
	ImGui::Text("History:");
	ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth()* 0.95, 200.0f), true);

	static int his_selected = -1;
	for (int n = 0; n < 5; n++)
	{
		char buf[32];
		sprintf(buf, "Object %d", n);
		if (ImGui::Selectable(buf, his_selected == n))
			his_selected = n;
	}
	// ImGui::TreePop();

	ImGui::EndChild();

	if (ImGui::Button("Undo", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {}
	ImGui::SameLine();
	if (ImGui::Button("Redo", ImVec2(ImGui::GetWindowWidth()* 0.45, 20.0f))) {}

	ImGui::Text("");
	ImGui::Text("Default:");
	ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(ImGui::GetWindowWidth()* 0.95, 200.0f), true);
	
	static int def_selected = -1;
	for (int n = 0; n < 5; n++)
	{
		char buf[32];
		sprintf(buf, "Object %d", n);
		if (ImGui::Selectable(buf, def_selected == n))
			def_selected = n;
	}
	//ImGui::TreePop();

	ImGui::EndChild();

	ImGui::End();
}

void Gui::showColorBar() {
	ImGui::Begin("Color Bar", &colorBar, 0 | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);

	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(260, screenHeight));
		ImGui::SetWindowPos(ImVec2(0, 18));
	}
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool options_menu = true;
	static bool hdr = false;
	int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
	ImGuiColorEditFlags flags = misc_flags;

	ImGui::ColorPicker4("MyColor##4", (float*)&cubes_color, flags, NULL);

	ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth() * 0.95, ImGui::GetWindowHeight() * 0.65), true);
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (j != 0) ImGui::SameLine();
			ImGui::PushID(i * 5 + j);
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f, 0.5f + j * 0.1f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f + 0.05f, 0.5f + j * 0.1f + 0.05f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 100.0f, 0.5f + j * 0.1f + 0.1f, 0.5f + j * 0.1f + 0.1f));
			if (ImGui::Button("     ", ImVec2(ImGui::GetWindowWidth() * 0.15, 20.0f))) {
				static ImVec4 saved_palette;
				ImGui::ColorConvertHSVtoRGB(i / 100.0f, 0.5f + j * 0.1f, 0.5f + j * 0.1f, saved_palette.x, saved_palette.y, saved_palette.z);
				cubes_color[0] = saved_palette.x;
				cubes_color[1] = saved_palette.y;
				cubes_color[2] = saved_palette.z;
			}
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
	}
	ImGui::EndChild();

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
}

void Gui::setMode_print() {
	mode = PAINT_MODE;
}

void Gui::setMode_erase() {
	mode = ERASE_MODE;
}