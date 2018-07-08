#include "Global.hpp"

OperationManager operationManager;

bool menuItem = true;
float cubes_color[4] = { 0.76f, 0.78f, 0.96f, 1.0f };
float background_color[4] = { 0.86f, 0.94f, 0.96f, 1.0f };
float deltaTime = 0.0f;
float lastFrame = 0.0f;
unsigned int numPerEdge = 20;
int mode = ERASE_MODE;

// interactive variables
int screenWidth = 960;
int screenHeight = 960;
bool isFirstDraw = true;

// Light variable
glm::vec3 lightPos = glm::vec3(1.5f, 1.0f, 1.5f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);