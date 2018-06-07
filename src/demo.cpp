#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CraftManager.hpp"
#include "CubeManager.hpp"

// interactive variables
int screenWidth = 1080;
int screenHeight = 960;

// About cubes
GLfloat sizePerCube = 0.1f;
unsigned int numPerEdge = 10;
const string mat4Name = "model";
vector<GLuint> attriSize;
GLfloat rotateSensivitiy = 30.0f;
GLfloat lookAroundSensivitiy = 1.0f;

// hover color
glm::vec3 hoverColor(1.0f, 0.0f, 0.0f);

// Camera class
static Camera* camera = Camera::getInstance();

bool isFpsMode = true;

// lighting
glm::vec3 lightPos(1.5f, 1.0f, 1.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 objectColor(cubes_color[0], cubes_color[1], cubes_color[2]);
glm::vec3 specular(0.2f, 0.2f, 0.2f);  // test material
// parameters
float shininess = 32.0f;

// callback functions
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// util functions
void PickOneCube(
	int xpos, int ypos,
	int screenWidth, int screenHeight,
	const glm::mat4& view,
	const glm::mat4& projection,
	unsigned int numPerEdge,
	float sizePerCube,
	CubeManager cubeManager,
	const glm::vec3& hoverColor
);


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
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "MineCube", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // vertical synchronization
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// initialize GLAD and load pointer address of OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	Shader phongShader("../src/Shader/phongvs.vs", "../src/Shader/phongfs.fs");
    
	Gui gui(window);

	// to normalize
	attriSize.push_back(3);
	CraftManager craftManger(sizePerCube, numPerEdge, phongShader.ID, mat4Name, attriSize);

    CubeManager cubeManager(numPerEdge, numPerEdge, numPerEdge, sizePerCube);

	cubeManager.defalut_init_all(phongShader.ID, mat4Name);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		gui.createNewFrame();
		gui.draw();

		glm::vec3 objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
		glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		phongShader.use();
        
		phongShader.setVec3("viewPos", camera->getCameraPosition());

        // material
        phongShader.setVec3("material.specular", specular);
        phongShader.setFloat("material.shininess", shininess);
        // light
        phongShader.setVec3("light.position",  lightPos);
        phongShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        phongShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        phongShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		glm::mat4 view = camera->getViewMatrix();
		phongShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoomFactor()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		phongShader.setMat4("projection", projection);

		/*
		-----------------------------------------------------------------------------------
			set/reset cubes
		-----------------------------------------------------------------------------------
		*/
		for (int index = 0; index < glm::pow(numPerEdge, 3); index++) {
			int t_index = index;
			int x = t_index / (int)glm::pow(numPerEdge, 2);
			t_index -= x * (int)glm::pow(numPerEdge, 2);
			int y = t_index / glm::pow(numPerEdge, 1);
			t_index -= y * (int)glm::pow(numPerEdge, 1);
			int z = t_index;

			auto cube = cubeManager.getCube(x, y, z);
			for (int plane = 0; plane < 6; plane++)
				cube->editColor(objectColor.x, objectColor.y, objectColor.z, plane);
		}

		/*
		-----------------------------------------------------------------------------------
			OBB-ray hitting test
		-----------------------------------------------------------------------------------
		*/
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		PickOneCube(
			(int)xpos, (int)ypos, (int)screenWidth, (int)screenHeight,
			view, projection, 
			numPerEdge, sizePerCube, 
			cubeManager, 
			hoverColor
		);

		/*
		-----------------------------------------------------------------------------------
		    move cubes
		-----------------------------------------------------------------------------------
		*/
		if (camera->isRotateX()) {
			cubeManager.setRotateSensivity(rotateSensivitiy);
			cubeManager.rotateHorizontal(camera->getRotateX());
			camera->resetRotateX();
		}

		if (camera->isRotateY()) {
			cubeManager.setRotateSensivity(rotateSensivitiy);
			cubeManager.rotateHorizontal(camera->getRotateY());
			camera->resetRotateY();
		}

		if (camera->isMoving) {
			cubeManager.setRotateSensivity(lookAroundSensivitiy);
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			glm::vec2 offset = camera->updateXYoffset((float)x, (float)y);
			cubeManager.rotateHorizontal(offset.x);
			cubeManager.rotateVertical(offset.y);
		}

		cubeManager.setAllShaderId(phongShader.ID);
        cubeManager.draw();
		gui.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gui.clear();


	glfwTerminate();
	return 0;
}

void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

