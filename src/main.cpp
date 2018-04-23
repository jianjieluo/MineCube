#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CraftManager.hpp"

#include <windows.h>
#ifdef _UNIX__
	#include <unistd.h>; // uncomment this statement to fix compilation on Unix
#endif


// interactive variables
int screenWidth = 800;
int screenHeight = 600;

GLfloat sizePerCube = 0.1;
unsigned int numPerEdge = 10;
const string mat4Name = "model";
vector<GLuint> attriSize;

// Camera class
Camera camera;
float deltaTime = 0.0f; // time between frames
float lastFrame = 0.0f;
bool isFpsMode = true;

// lighting
glm::vec3 lightPos(0.4f, 0.2f, 1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);

// parameters
float ambientStrength = 0.1f;
float specularStrength = 0.5f;
float shininess = 32.0f;

// callback functions
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main()
{
	glfwSetErrorCallback(glfw_error_callback);

	// initialize GLFW
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    // creat GLFW window
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "CG HW4", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vertical synchronization
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// initialize GLAD and load pointer address of OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);



	Shader phongShader("shader/phongvs.vs", "shader/phongfs.fs");
	Gui gui(window);

	// to normalize
	attriSize.push_back(3);
	CraftManager craftManger(sizePerCube, numPerEdge, phongShader.ID, mat4Name, attriSize);
	float data[108] = {
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,

		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,

		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};
	vector<float> vertexAttrArray(data, data + 108);
	
	craftManger.setAttriArray(0, 3, vertexAttrArray);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		gui.createNewFrame();
		gui.draw();

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (isFpsMode) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			camera.pause();
		}

		phongShader.use();
		phongShader.setVec3("objectColor", objectColor);
		phongShader.setVec3("lightColor", lightColor);
		phongShader.setVec3("lightPos", lightPos);
		phongShader.setVec3("viewPos", camera.getCameraPosition());
		phongShader.setFloat("ambientStrength", ambientStrength);
		phongShader.setFloat("specularStrength", specularStrength);
		phongShader.setFloat("shininess", shininess);

		//glm::mat4 model;
		//phongShader.setMat4("model", model);
		glm::mat4 view = camera.getViewMatrix();
		phongShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoomFactor()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		phongShader.setMat4("projection", projection);


		// draw
		craftManger.draw();
		
		gui.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gui.clear();


	glfwTerminate();
	return 0;
}

void draw_GUI(GLFWwindow *window) {

}

void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		isFpsMode = !isFpsMode;

		// to prevent multi press
#ifdef __WIN32__
		Sleep(200); // uncomment this statement to fix compilation on window
#endif
#ifdef __UNIX__
		usleep(200000); // uncomment this statement to fix compilation on unix
#endif
	}
	if (isFpsMode) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.moveCamera(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.moveCamera(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.moveCamera(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.moveCamera(RIGHT, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double cur_x, double cur_y) {
	if (isFpsMode) {
		camera.lookAround(cur_x, cur_y);
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	std::cout << "hello" << std::endl;

	camera.zoomInOrOut(yoffset);
}