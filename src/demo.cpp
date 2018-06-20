#include "Global.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Gui.hpp"
#include "CubeManager.hpp"
#include "SkyBox.hpp"

#include <iostream>
using namespace std;

#define SHADOW
//#define PLANE
#define DEBUG

// About cubes
GLfloat sizePerCube = 0.1f;
unsigned int numPerEdge = 10;
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

// add/delete/paint
glm::vec3 startCubePos(0.0f, 0.0f, 0.0f);

// Camera class
static Camera* camera = Camera::getInstance();

// lighting
glm::vec3 lightPos(1.5f, 1.0f, 1.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 objectColor(cubes_color[0], cubes_color[1], cubes_color[2]);
glm::vec3 objectColorLast(cubes_color[0], cubes_color[1], cubes_color[2]);
bool no_reset = false;


glm::vec3 specular(0.2f, 0.2f, 0.2f);  // test material
// parameters
float shininess = 32.0f;

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
void setAllCubesColor(CubeManager& cubeManager, glm::vec3 color);

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

	SkyBox skybox(window, camera);

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
	CraftManager craftManger(sizePerCube, numPerEdge, phongShader.ID, mat4Name, attriSize);

    CubeManager cubeManager(numPerEdge, numPerEdge, numPerEdge, sizePerCube);

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
        phongShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        phongShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        phongShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

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
			*	set/reset hovered cube
			*
			***********************************************************************************/
			// set colors of all cubes
			//if (objectColor.x != objectColorLast.x || objectColor.y != objectColorLast.y || objectColor.z != objectColorLast.z) {
			//	setAllCubesColor(cubeManager, objectColor);
			//	objectColorLast = objectColor;
			//}

			// reset last hovered plane
			auto last_hover_cube = cubeManager.getCube(static_cast<int>(hoverCubePosLast.x), static_cast<int>(hoverCubePosLast.y), static_cast<int>(hoverCubePosLast.z));
			if (last_hover_cube && hoverPlaneLast != -1) {
				last_hover_cube->editColor(objectColorLast.x, objectColorLast.y, objectColorLast.z, hoverPlaneLast);
			}

			// set hovered plane
			auto hover_cube = cubeManager.getCube(hoverCubePosCurrent.x, hoverCubePosCurrent.y, hoverCubePosCurrent.z);
			if (hover_cube) {
				hoverCubePosLast = hoverCubePosCurrent;
				objectColorLast = hover_cube->getColorOfPLane(hoverPlaneCurrent);
				hover_cube->editColor(hoverColor.x, hoverColor.y, hoverColor.z, hoverPlaneCurrent);
			}


			/**********************************************************************************
			*
			*	deal with some cubes
			*
			***********************************************************************************/
			if (hit) {
				if (mouseJustClick && !mouseIsDown) {
					if (mode == ERASE_MODE || mode == PAINT_MODE) {
						// 记录当前悬浮方块
						startCubePos = hoverCubePosCurrent;
					}
					if (mode == CREATE_MODE) {
						// 获取交点所在的面
						auto sptr = shared_ptr<Cube>(new Cube(sizePerCube, phongShader.ID, mat4Name, attriSize));
						objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
						for (int i = 0; i < 6; i++)
							sptr->editColor(objectColor.x, objectColor.y, objectColor.z, i);

						int new_x = static_cast<int>(hoverCubePosCurrent.x), new_y = static_cast<int>(hoverCubePosCurrent.y), new_z = static_cast<int>(hoverCubePosCurrent.z);
						switch (hoverPlaneCurrent) {
						case 0: new_z -= 1; break;
						case 1: new_z += 1; break;
						case 2: new_x -= 1; break;
						case 3: new_x += 1; break;
						case 4: new_y -= 1; break;
						case 5: new_y += 1; break;
						default: break;
						}
						if (new_x > -1 && new_y > -1 && new_z > -1 && new_x < numPerEdge && new_y < numPerEdge && new_z < numPerEdge)
							cubeManager.setCube(new_x, new_y, new_z, sptr);
					}
					mouseIsDown = true;
				}
				if (mouseJustRelease && mouseIsDown) {
					if (mode == ERASE_MODE || mode == PAINT_MODE) {
						// 获取当前悬浮方块，计算消除
						unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(hoverCubePosCurrent.x));
						unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(hoverCubePosCurrent.y));
						unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(hoverCubePosCurrent.z));
						unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(hoverCubePosCurrent.x));
						unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(hoverCubePosCurrent.y));
						unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(hoverCubePosCurrent.z));
						for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
							for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
								for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
									if (mode == ERASE_MODE)  cubeManager.deleteCube(i, j, k);
									if (mode == PAINT_MODE) {
										auto cube = cubeManager.getCube(i, j, k);
										if (cube) {
											objectColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
											for (int plane = 0; plane < 6; plane++)
												cube->editColor(objectColor.x, objectColor.y, objectColor.z, plane);
										}
									}
								}
					}
					mouseIsDown = false;
				}

			}


			/**********************************************************************************
			*
			*	move camera
			*
			***********************************************************************************/
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

			/**********************************************************************************
			*
			*	update variables
			*
			***********************************************************************************/
			hoverPlaneLast = hoverPlaneCurrent;
		}



		cubeManager.setAllShaderId(phongShader.ID);

		// RenderScene(phongShader, cubeManager);
		// cubeManager.draw();
		skybox.render();

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

void setAllCubesColor(CubeManager& cubeManager, glm::vec3 color) {
	for (int index = 0; index < glm::pow(numPerEdge, 3); index++) {
		int t_index = index;
		int x = t_index / (int)glm::pow(numPerEdge, 2);
		t_index -= x * (int)glm::pow(numPerEdge, 2);
		int y = t_index / glm::pow(numPerEdge, 1);
		t_index -= y * (int)glm::pow(numPerEdge, 1);
		int z = t_index;

		auto cube = cubeManager.getCube(x, y, z);
		if (cube)
			for (int plane = 0; plane < 6; plane++)
				cube->editColor(color.x, color.y, color.z, plane);
	}
}