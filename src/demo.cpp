#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CubeManager.hpp"
#include "Cube.hpp"
#include "SkyBox.hpp"
#include "Cloth.hpp"
#include "CRUD.h"
#include "Text.hpp"

#include <iostream>
#include <list>
#include <ctime>

using namespace std;

//#define CALTIME  // calculate time/efficiency

// About cubes
GLfloat sizePerCube = 0.05f;
const string mat4Name = "model";
vector<GLuint> attriSize;
GLfloat rotateSensivitiy = 30.0f;
GLfloat lookAroundSensivitiy = 1.0f;

// About picking
glm::vec3 hoverColor(1.0f, 0.0f, 0.0f);
glm::vec3 hoverCubePosCurrent(0.0f, 0.0f, 0.0f);
glm::vec3 hoverCubePosLast(0.0f, 0.0f, 0.0f);
int hoverPlaneCurrent = -1;
int hoverPlaneLast = -1;
bool isHitBefore = false;

// add/delete/paint
glm::vec3 startCubePos(0.0f, 0.0f, 0.0f);
glm::vec3 farCubePos(0.0f, 0.0f, 0.0f);
glm::vec3 auxColor(0.9f, 0.9f, 1.0f);

// Camera class
static Camera* camera = Camera::getInstance();

bool no_reset = false;

// parameters
glm::vec3 specular(0.7f, 0.7f, 0.07f);
float shininess = 1024.0f;

int main();

// callback functions
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void RenderScene(Shader &phongShader, CubeManager & cubeManager);

// util functions
bool PickOneCube(
	int xpos, int ypos,
	int screenWidth, int screenHeight,
	const glm::mat4& view,
	const glm::mat4& projection,
	unsigned int numPerEdge,
	float sizePerCube,
	CubeManager cubeManager,
	glm::vec3& hoverCubeCurrent,
	int& plane_num_current
);

/**********************************************************************************
*
* Work Bar(帧缓存特效相关)
* @author mgsweet <mgsweet@126.com>
*
***********************************************************************************/
float quadVertices[] = { 
	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};
GLuint workBarFBO = 1;
unsigned int workBarColorBuffer;
GLuint workBarRBO;
unsigned int quadVAO, quadVBO; // screen quad VAO

void initWorkBar();
void updateWorkBar();


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

	Shader phongShader("../src/Shader/phongvs.vs", "../src/Shader/phongfs.fs");
	Shader simpleDepthShader("../src/Shader/shadow_mapping_depth.vs", "../src/Shader/shadow_mapping_depth.fs");

	// 帧缓存，特效
	Shader screenShader("../src/Shader/framebuffers_screen.vs", "../src/Shader/framebuffers_screen.fs");

	SkyBox skybox(window, camera);

	Cloth cloth(window, lightPos, lightColor, screenWidth, screenHeight);
	int timestep = 0;

	Text text(window, camera);
	text.push("Mine Cube", glm::vec3(1.0f, 1.0f, 1.0f), 50);
	Gui gui(window, &text);

    // Shadow  config
    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
	// Configure depth map FBO
    const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // shadow configuration
    // --------------------
    phongShader.use();
    phongShader.setInt("shadowMap", 0); // In phongshader we just use 1 texture, TEXTURE0 is default.

	// 帧缓存特效相关
	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	initWorkBar();

	// to normalize
	attriSize.push_back(3);

    CubeManager cubeManager(numPerEdge, numPerEdge, numPerEdge, sizePerCube);

	// for save/load model
	gui.setPtrCubeManager(&cubeManager);

	cubeManager.defalut_init_all(phongShader.ID, mat4Name);

    // Open Face Culling
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	bool hit = false;

#ifdef CALTIME
	clock_t tic, toc;
#endif // CALTIME
	// main loop
	while (!glfwWindowShouldClose(window))
	{
		updateWorkBar();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;	
		lastFrame = currentFrame;

		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shadow support
		// -------------------------------------------
        // 1. Render depth of scene to texture (from light's perspective)
        // - Get light projection/view matrix.
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 1.0f, far_plane = 7.5f;

        // Orthographic 
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // - render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        RenderScene(simpleDepthShader, cubeManager);

		// 这里先不到原来的帧中渲染，先渲染到帧缓存中
		glBindFramebuffer(GL_FRAMEBUFFER, workBarFBO);

        // -----------------------------------------
        // reset viewport
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        phongShader.use();
		phongShader.setVec3("viewPos", camera->getCameraPosition());

        // material
        phongShader.setVec3("material.specular", specular);
        phongShader.setFloat("material.shininess", shininess);
        // light
        phongShader.setVec3("light.position",  lightPos);
        phongShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
        phongShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        phongShader.setVec3("light.specular", 0.3f, 0.3f, 0.3f);


		phongShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		auto view = camera->getViewMatrix();
		phongShader.setMat4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(camera->getZoomFactor()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		phongShader.setMat4("projection", projection);

        glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(phongShader, cubeManager);

		// hover support
		// ---------------------------------------------------------------
		// reset last hovered plane
		if (hit && static_cast<int>(hoverCubePosLast.x) < numPerEdge && static_cast<int>(hoverCubePosLast.y) < numPerEdge && static_cast<int>(hoverCubePosLast.z) < numPerEdge) {
			auto last_hover_cube = cubeManager.getCube(static_cast<int>(hoverCubePosLast.x), static_cast<int>(hoverCubePosLast.y), static_cast<int>(hoverCubePosLast.z));
			if (last_hover_cube)
				last_hover_cube->unhitted();
		}


		if (camera->isFpsMode) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			/**********************************************************************************
			*
			*	OBB-ray hitting test
			*
			***********************************************************************************/
#ifdef CALTIME
			tic = clock();
#endif // CALTIME

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			/*
			[return]:
			if_hit: bool, if the ray hit one of the cubes
			[output]:
			hoverCubePosCurrent: glm::vec3, (x, y, z) of the hit cube
			hoverPlaneLast: int, number of the plane of the hit cube
			*/
			hit = PickOneCube(
				(int)xpos, (int)ypos, (int)screenWidth, (int)screenHeight,
				view, projection,
				numPerEdge, sizePerCube,
				cubeManager,
				hoverCubePosCurrent,
				hoverPlaneCurrent
			);

#ifdef CALTIME
			toc = clock();
			cout << "It took " << (double)(toc - tic) / CLOCKS_PER_SEC << "  millisecond(s) to pick." << endl;
#endif // CALTIME


			/**********************************************************************************
			*
			*	run CRUD
			*
			***********************************************************************************/		
			CRUD::run(hit, gui, startCubePos, farCubePos,
				hoverCubePosCurrent, isHitBefore, hoverPlaneLast,
				hoverPlaneCurrent, cubeManager, hoverCubePosLast,
				phongShader);

			/**********************************************************************************
			*
			*	move camera
			*
			***********************************************************************************/
			if (!gui.isSaveWindowShow()) {
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
					cubeManager.rotateHorizontal(offset.x * 0.25);
					cubeManager.rotateVertical(offset.y * 0.25);
				}
			}
		}

		cubeManager.setAllShaderId(phongShader.ID);

		skybox.render();

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		screenShader.use();
		screenShader.setInt("specialEffect", gui.getSpecialEffect());

		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, workBarColorBuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// gui should render after all above
		if (gui.isClothAllow()) cloth.render(camera, timestep++);
		text.render();
		gui.createNewFrame();
		gui.draw();
		gui.render();


#ifdef __APPLE__
		// resolution for retina display issue
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		screenWidth = width;
		screenHeight = height;
#endif
		
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
	screenWidth = width;
	screenHeight = height;
	isFirstDraw = true;
}

void RenderScene(Shader &shader, CubeManager & cubeManager)
{
    // Cubes
    cubeManager.draw();
}

void initWorkBar() {
	glGenFramebuffers(1, &workBarFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, workBarFBO);

	//create a color attachment texture
	glGenTextures(1, &workBarColorBuffer);
	glBindTexture(GL_TEXTURE_2D, workBarColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, workBarColorBuffer, 0);

	glGenRenderbuffers(1, &workBarRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, workBarRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, workBarRBO); // now actually attach it

																										 // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void updateWorkBar() {
	glBindTexture(GL_TEXTURE_2D, workBarColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glBindRenderbuffer(GL_RENDERBUFFER, workBarRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
}