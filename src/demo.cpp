#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CraftManager.hpp"
#include "CubeManager.hpp"
// interactive variables
int screenWidth = 1080;
int screenHeight = 960;

GLfloat sizePerCube = 0.1f;
unsigned int numPerEdge = 10;
const string mat4Name = "model";
vector<GLuint> attriSize;
GLfloat rotateSensivitiy = 30.0f;
GLfloat lookAroundSensivitiy = 1.0f;

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
void ScreenPosToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, \
						glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec3& out_origin, glm::vec3& out_direction);
bool TestRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, glm::vec3 aabb_min, glm::vec3 aabb_max, \
							glm::mat4 ModelMatrix, float& intersection_distance);

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
        phongShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f); // a little darker to match the scene
        phongShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		glm::mat4 view = camera->getViewMatrix();
		phongShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoomFactor()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		phongShader.setMat4("projection", projection);

		/*
		-----------------------------------------------------------------------------------
			set cubes
		-----------------------------------------------------------------------------------
		*/
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		glm::vec3 ray_origin;
		glm::vec3 ray_direction;
		ScreenPosToWorldRay(xpos, screenHeight - ypos, screenWidth, screenHeight, \
							view, projection, ray_origin, ray_direction);
		//std::cout << ray_origin.x << ' ' << ray_origin.y << ' ' << ray_origin.z << '\n';
		//std::cout << ray_direction.x << ' ' << ray_direction.y << ' ' << ray_direction.z << '\n';
		//ray_direction = ray_direction*20.0f;

		int cube_num = -1;

		// Test each Oriented Bounding Box (OBB).
		for (int index = 0; index < glm::pow(numPerEdge, 3); index++) {
			int t_index = index;
			int x = t_index / (int)glm::pow(numPerEdge, 2);
			t_index -= x * (int)glm::pow(numPerEdge, 2);
			int y = t_index / glm::pow(numPerEdge, 1);
			t_index -= y * (int)glm::pow(numPerEdge, 1);
			int z = t_index;

			float intersection_distance; // Output of TestRayOBBIntersection()
			// Original vertices Coordinate
			// TODO: relative to {sizePerCube}
			glm::vec3 aabb_min(-0.1f, -0.1f, -0.1f);
			glm::vec3 aabb_max(0.1f, 0.1f, 0.1f);

			// The ModelMatrix transforms :
			// - Model transformation for each cube
			// - but also the AABB (defined with aabb_min and aabb_max) into an OBB
			glm::mat4 model_mat = cubeManager.getModelMat4(x, y, z);

			if (TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max,
				model_mat, intersection_distance)) {
				cube_num = index;
				break;
			}
		}

		//if (cube_num == -1)
		//	std::cout << "background\n";
		//else
		//	std::cout << "cube no." << cube_num << "is selected\n";
		// hit
		if (cube_num != -1) {
			int x = cube_num / (int)glm::pow(numPerEdge, 2);
			cube_num -= x * (int)glm::pow(numPerEdge, 2);
			int y = cube_num / glm::pow(numPerEdge, 1);
			cube_num -= y * (int)glm::pow(numPerEdge, 1);
			int z = cube_num;

			auto hover_cube = cubeManager.getCube(x, y, z);
			for (int plane = 0; plane < 6; plane++)
				hover_cube->editColor(1.0f, 0.0f, 0.0f, plane);
			cubeManager.setCube(x, y, z, hover_cube);
		}



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

		// draw
//        craftManger.draw();
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

