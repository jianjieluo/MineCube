#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// global variables for gui.cpp and main.cpp are declared here.
extern bool showGuiWindow;
extern bool menuItem;
extern float cubes_color[4];
extern float background_color[4];
extern float deltaTime;
extern float lastFrame;

#endif  // GLOBAL_HPP
