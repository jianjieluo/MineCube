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
	void draw(unsigned int textureColorbuffer);
	void render();
	void clear();
	void autoRePos();

	float* getWorkBarPos();

private:
	GLFWwindow* window;
	float workBarPos[2];
	bool colorBar;
	bool editBar;
	bool workBar;
	void showAppMainMenuBar();
	void showEditBar();
	void showColorBar();
	void showWorkBar(unsigned int textureColorbuffer);
	void setMode_add();
	void setMode_print();
	void setMode_erase();
	void captureKey();
};
