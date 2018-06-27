#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Global.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Camera.hpp"
#include "CubeManager.hpp"
#include <vector>

using namespace std;

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

	bool isClothAllow();
	bool isSaveWindowShow();

	float* getWorkBarPos();

	void setPtrCubeManager(CubeManager*);

private:
	ImVec4 latest_color;
	bool allowCloth;
	Camera* camera;
	CubeManager* ptr_cubeManager;
	GLFWwindow* window;
	vector<ImVec4> colorHistory;
	void addColor2His(ImVec4 hisColor);
	float workBarPos[2];
	bool colorBar;
	bool editBar;
	bool workBar;
	bool saveWindow;
	void showAppMainMenuBar();
	void showEditBar();
	void showColorBar();
	void showWorkBar(unsigned int textureColorbuffer);
	void setMode_add();
	void setMode_print();
	void setMode_erase();
	void captureKeys();
};
