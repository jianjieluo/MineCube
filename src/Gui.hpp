#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Global.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

extern bool showGuiWindow;
extern bool menuItem;

class Gui {
public:
	// constructor, ImGui initialization and bindings
	Gui(GLFWwindow* theWindow);

	void createNewFrame();
	void draw();
	void render();
	void clear();

private:
	GLFWwindow* window;
};