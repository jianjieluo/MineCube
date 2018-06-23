#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OperationManager.hpp"

#define ERASE_MODE 0
#define CREATE_MODE 1
#define PAINT_MODE 2

// global manager controls operation
extern OperationManager operationManager;

// global variables for gui.cpp and main.cpp are declared here.
extern bool showGuiWindow;
extern bool menuItem;
extern float cubes_color[4];
extern float background_color[4];
extern float deltaTime;
extern float lastFrame;
extern int mode;
extern bool mouseJustClick;
extern bool mouseJustRelease;
extern bool mouseIsDown;
extern bool isFirstDraw;

// interactive variables
extern int screenWidth;
extern int screenHeight;

#endif  // GLOBAL_HPP
