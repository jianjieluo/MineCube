#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Global.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

class Gui {
public:
	// constructor, ImGui initialization and bindings
	Gui(GLFWwindow* theWindow);

	void createNewFrame();
	void draw();
	void render();
	void clear();
	void autoRePos();

private:
	GLFWwindow* window;
	bool colorBar;
	bool editBar;
	bool workBar;
	void showAppMainMenuBar();
	void showEditBar();
	void showColorBar();
	void showWorkBar();
	void setMode_add();
	void setMode_print();
	void setMode_erase();
};
