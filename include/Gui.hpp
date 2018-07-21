#ifndef GUI_HPP
#define GUI_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Global.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Camera.hpp"
#include "CubeManager.hpp"
#include "Text.hpp"
#include <vector>

using namespace std;

class Gui {
public:
	// constructor, ImGui initialization and bindings
	Gui(GLFWwindow* theWindow, Text* _text);
	void createNewFrame();
	void draw();
	void draw(unsigned int textureColorbuffer);
	void render();
	void clear();
	void autoRePos();
	bool isFullNeed;
	bool isClothAllow();
	bool isSaveWindowShow();
	float* getWorkBarPos();
	void setPtrCubeManager(CubeManager*);
	int getSpecialEffect();
	void updateSE(Shader* shader);
	bool allowCloth;


private:
	int specialEffect;
	Text* text;
	ImVec4 latest_color;
	Camera* camera;
	CubeManager* ptr_cubeManager;
	GLFWwindow* window;
	vector<ImVec4> colorHistory;
	void addColor2His(ImVec4 color);
	float workBarPos[2];
	bool colorBar;
	bool editBar;
	bool workBar;
	bool saveWindow;
	void showEditBar();
	void showColorBar();
	void showWorkBar(unsigned int textureColorbuffer);
	void setMode_add();
	void setMode_print();
	void setMode_erase();
	void captureKeys();
	void showAppMainMenuBar();
};

#endif GUI_HPP
