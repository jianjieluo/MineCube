#ifndef SKYBOX_HPP
#define SKUBOX_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "Shader.hpp"
#include "Camera.hpp"

class SkyBox {
    private:
		GLFWwindow * window;
		Camera* camera;
		Shader* skyboxShader;
		unsigned int cubemapTexture;
		unsigned int skyboxVAO, skyboxVBO;

		float skyboxVertices[3 * 6 * 6];
    public:
        SkyBox(GLFWwindow* theWindow, Camera* theCamera);
        void render();
		void clean();
};

#endif