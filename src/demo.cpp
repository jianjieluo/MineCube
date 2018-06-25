#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CubeManager.hpp"
#include "Cube.hpp"
#include "SkyBox.hpp"
#include "Cloth.hpp"
#include "BasicOperation.hpp"
#include "OperationManager.hpp"
#include "crud.h"

#include <iostream>
#include <list>

using namespace std;

#define SHADOW
//#define PLANE
#define DEBUG

// About cubes
GLfloat sizePerCube = 0.05f;
unsigned int numPerEdge = 20;
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

// lighting
glm::vec3 lightPos(1.5f, 1.0f, 1.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 objectColor(cubes_color[0], cubes_color[1], cubes_color[2]);
bool no_reset = false;

// parameters
glm::vec3 specular(0.7f, 0.7f, 0.07f);
float shininess = 1024.0f;

int main();

// callback functions
void glfw_error_callback(int error, const char* description);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


#ifdef PLANE
GLuint planeVAO;
#endif

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

    // Define the viewport dimensions
#ifdef SHADOW
    glViewport(0, 0, screenWidth, screenHeight);
#endif

	glEnable(GL_DEPTH_TEST);

	Shader phongShader("../src/Shader/phongvs.vs", "../src/Shader/phongfs.fs");

#ifdef SHADOW	
	Shader simpleDepthShader("../src/Shader/shadow_mapping_depth.vs", "../src/Shader/shadow_mapping_depth.fs");
    Shader debugDepthQuad("../src/Shader/debug_quad_depth.vs", "../src/Shader/debug_quad_depth.fs");
#endif

	Gui gui(window);

	//use to capture io
	ImGuiIO& io = ImGui::GetIO();

	SkyBox skybox(window, camera);

	Cloth cloth(window, lightPos, lightColor, screenWidth, screenHeight);
	int timestep = 0;

#ifdef PLANE
	// set floor
    GLfloat planeVertices[] = {
        // Positions          // Normals       
        25.0f, -0.7f, 25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.7f, -25.0f, 0.0f, 1.0f, 0.0f, 
        -25.0f, -0.7f, 25.0f, 0.0f, 1.0f, 0.0f,

        25.0f, -0.7f, 25.0f, 0.0f, 1.0f, 0.0f,
        25.0f, -0.7f, -25.0f, 0.0f, 1.0f, 0.0f,
        -25.0f, -0.7f, -25.0f, 0.0f, 1.0f, 0.0f
    };
    // Setup plane VAO
    GLuint planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
#endif

#ifdef SHADOW
	// Configure depth map FBO
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 防止纹理贴图在远处重复渲染
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // phongShader configuration
    // --------------------
    phongShader.use();
    phongShader.setInt("diffuseTexture", 0);
    phongShader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
#endif


/*-------------------Work Bar--------------------------*/
	GLuint workBarFBO = 0;
	glGenFramebuffers(1, &workBarFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, workBarFBO);

	//create a color attachment texture
	unsigned int workBarColorBuffer;
	glGenTextures(1, &workBarColorBuffer);
	glBindTexture(GL_TEXTURE_2D, workBarColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, workBarColorBuffer, 0);

	GLuint workBarRBO;
	glGenRenderbuffers(1, &workBarRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, workBarRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, workBarRBO); // now actually attach it

	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


/*----------------------end workbar----------------*/

	// to normalize
	attriSize.push_back(3);

    CubeManager cubeManager(numPerEdge, numPerEdge, numPerEdge, sizePerCube);

	// for save/load model
	gui.setPtrCubeManager(&cubeManager);

	cubeManager.defalut_init_all(phongShader.ID, mat4Name);

	// main loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;	
		lastFrame = currentFrame;

		glm::vec3 objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
		glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef SHADOW
		// -------------------------------------------
        // 1. Render depth of scene to texture (from light's perspective)
        // - Get light projection/view matrix.
        glCullFace(GL_FRONT);
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
        glActiveTexture(GL_TEXTURE0);
        RenderScene(simpleDepthShader, cubeManager);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// bind to framebuffer and draw scene as we normally would to color texture 
		//glBindFramebuffer(GL_FRAMEBUFFER, workBarFBO);
		//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)



        glCullFace(GL_BACK); // 不要忘记设回原先的culling face

        // -----------------------------------------
        // reset viewport
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#endif

		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		glm::mat4 view = camera->getViewMatrix();
		phongShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera->getZoomFactor()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		phongShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(phongShader, cubeManager);

#ifdef SHADOW
		// render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // RenderQuad();
#endif

		// reset last hovered plane
		auto last_hover_cube = cubeManager.getCube(static_cast<int>(hoverCubePosLast.x), static_cast<int>(hoverCubePosLast.y), static_cast<int>(hoverCubePosLast.z));
		if (last_hover_cube) {
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

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			/*
			[return]:
			if_hit: bool, if the ray hit one of the cubes
			[output]:
			hoverCubePosCurrent: glm::vec3, (x, y, z) of the hit cube
			hoverPlaneLast: int, number of the plane of the hit cube
			*/
			bool hit = PickOneCube(
				(int)xpos, (int)ypos, (int)screenWidth, (int)screenHeight,
				view, projection,
				numPerEdge, sizePerCube,
				cubeManager,
				hoverCubePosCurrent,
				hoverPlaneCurrent
			);

			/**********************************************************************************
			*
			*	deal with some cubes
			*	以下代码执行顺序很重要，掉乱顺序有可能造成斑点
			*
			***********************************************************************************/
			if (hit && !gui.isSaveWindowShow()) {
				//若点击左键
				if (ImGui::IsMouseClicked(0)) {
					if (mode == ERASE_MODE || mode == PAINT_MODE) {
						// 记录当前悬浮方块
						startCubePos = hoverCubePosCurrent;
						farCubePos = hoverCubePosCurrent;

						//用于拉出框体的操作
						isHitBefore = true;
					}
					if (mode == CREATE_MODE) {
						objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
						// 保全当前全局变量
						const int constHoverPlane = hoverPlaneCurrent;
						const glm::vec3 constCubePos = hoverCubePosCurrent;
						// 先判断能否创建再执行
						if (canCreate(cubeManager, hoverCubePosCurrent, hoverPlaneCurrent, numPerEdge)) {
							auto createOP = shared_ptr<BasicOperation>(new BasicOperation(
								//	do 
								[&cubeManager, &objectColor, phongShader]() {
								createCube(cubeManager, hoverCubePosCurrent, hoverPlaneCurrent, objectColor, phongShader.ID, numPerEdge);
							},
								//	undo
								[&cubeManager, constCubePos, constHoverPlane]() {
								undoAdd(cubeManager, constCubePos, constHoverPlane);
							}
							));
							operationManager.executeOp(createOP, "Create a cube");
						}
					}
				}
				//若按着左键
				if (io.MouseDown[0]) {
					if ((mode == PAINT_MODE || mode == ERASE_MODE) && farCubePos != hoverCubePosCurrent && isHitBefore) {
						unselectCubes(cubeManager, startCubePos, farCubePos);
						selectCubes(cubeManager, startCubePos, hoverCubePosCurrent);
						farCubePos = hoverCubePosCurrent;
					}
				}

				// set hovered plane
				auto hover_cube = cubeManager.getCube(hoverCubePosCurrent.x, hoverCubePosCurrent.y, hoverCubePosCurrent.z);
				if (!hover_cube->isDeleted()) {
					hoverCubePosLast = hoverCubePosCurrent;
					hover_cube->hitted();
				}
			} 

			//若松开左键
			if (ImGui::IsMouseReleased(0)) {
				if (isHitBefore) {
					if (mode == ERASE_MODE) {
						// 保全当前全局变量
						const glm::vec3 constStartCubePos = startCubePos;
						const glm::vec3 constEndCubePos = hoverCubePosCurrent;
						const vector<CubeInfo> constCubesInfo = getCubesInfo(cubeManager, constStartCubePos, constEndCubePos);
						auto eraseOP = shared_ptr<BasicOperation>(new BasicOperation(
							//	do 
							[&cubeManager, constStartCubePos, constEndCubePos]() {
							//	去除辅助色
							unselectCubes(cubeManager, startCubePos, farCubePos);

							eraseCube(cubeManager, constStartCubePos, constEndCubePos);
						},
							//	undo
							[&cubeManager, constStartCubePos, constEndCubePos, constCubesInfo]() {
							undoErase(cubeManager, constStartCubePos, constEndCubePos, constCubesInfo);
						}
						));
						operationManager.executeOp(eraseOP, "Erase a cubes");
					}
					if (mode == PAINT_MODE) {
						// 保全当前全局变量
						const glm::vec3 constStartCubePos = startCubePos;
						const glm::vec3 constEndCubePos = hoverCubePosCurrent;
						const vector<CubeInfo> constCubesInfo = getCubesInfo(cubeManager, constStartCubePos, constEndCubePos);
						auto paintOP = shared_ptr<BasicOperation>(new BasicOperation(
							//	do 
							[&cubeManager, &objectColor, constStartCubePos, constEndCubePos]() {
							//	去除辅助色
							unselectCubes(cubeManager, startCubePos, farCubePos);

							objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
							paintCube(cubeManager, constStartCubePos, constEndCubePos, objectColor);
						},
							//	undo
							[&cubeManager, constStartCubePos, constEndCubePos, constCubesInfo]() {
							undoPaint(cubeManager, constStartCubePos, constEndCubePos, constCubesInfo);
						}
						));
						operationManager.executeOp(paintOP, "Change cubes color");
					}
				}
				isHitBefore = false;
			}

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

		// RenderScene(phongShader, cubeManager);
		// cubeManager.draw();
		skybox.render();
		if (gui.isClothAllow()) cloth.render(camera, timestep++);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		//						  // clear all relevant buffers
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		//glClear(GL_COLOR_BUFFER_BIT);


		gui.createNewFrame();
		gui.draw();

		/*ImGui::Begin("Work Bar");
		ImGui::Image((void*)(intptr_t)workBarColorBuffer, ImVec2(SCR_WIDTH, SCR_HEIGHT), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
		ImGui::End();*/

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
	screenWidth = width;
	screenHeight = height;
	isFirstDraw = true;
}

void RenderScene(Shader &shader, CubeManager & cubeManager)
{
#ifdef PLANE
    // Floor
    glm::mat4 model;
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
#endif // PLANE
    // Cubes
    cubeManager.draw();
    // model = glm::mat4();
    // //model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    // phongShader.setMat4("model", glm::value_ptr(model));
    // phongShader.setFloat3("objectColor", glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    // glBindVertexArray(cubeVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0);
}