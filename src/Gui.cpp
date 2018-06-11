#include "Gui.hpp"

Gui::Gui(GLFWwindow* theWindow) {
	// ImGui initialization and bindings
	window = theWindow;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	isFirstDraw = true;
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
				// TODO
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
			ImGui::Text("Hint: Press 'V' to switch Camera View");
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Gui::showEditBar() {
	ImGui::Begin("Edit Bar", &editBar, ImGuiWindowFlags_NoScrollbar);

	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(260, 300));
		ImGui::SetWindowPos(ImVec2(800, 20));
	}

	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	if (ImGui::ArrowButton("left", ImGuiDir_Left)) {}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("right", ImGuiDir_Right)) {}
	ImGui::SameLine(0.0f, 4 * spacing);

	if (ImGui::Button(" + ")) {};
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::Button(" - ")) {};
	// List box
	const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
	static int listbox_item_current = 1;
	ImGui::ListBox("History", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items));

	ImGui::End();
}

void Gui::showColorBar() {
	ImGui::Begin("File Bar", &colorBar, 0);
	if (isFirstDraw) {
		ImGui::SetWindowSize(ImVec2(260, 750));
		ImGui::SetWindowPos(ImVec2(0, 20));
	}
	static bool alpha_preview = true;
	static bool alpha_half_preview = false;
	static bool options_menu = true;
	static bool hdr = false;
	int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
	ImGuiColorEditFlags flags = misc_flags;

	ImGui::ColorPicker4("MyColor##4", (float*)&cubes_color, flags, NULL);

	// Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
	for (int i = 0; i < 100; i++)
	{
		if (i % 5 != 0) ImGui::SameLine();
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 100.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 100.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 100.0f, 0.8f, 0.8f));
		if (ImGui::Button("     ")) {
			static ImVec4 saved_palette;
			ImGui::ColorConvertHSVtoRGB(i / 100.0f, 0.6f, 0.6f, saved_palette.x, saved_palette.y, saved_palette.z);
			cubes_color[0] = saved_palette.x;
			cubes_color[1] = saved_palette.y;
			cubes_color[2] = saved_palette.z;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}

	ImGui::End();
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