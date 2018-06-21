#ifndef CLOTH_H
#define CLOTH_H

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

class Cloth {
    private:
		GLFWwindow * window;
		Shader* clothShader;
		glm::vec3 lightPos;
		glm::vec3 lightColor;
		float SCR_WIDTH;
		float SCR_HEIGHT;

		unsigned int clothVAO, clothVBO, clothEBO;

		float clothVertices[20 * 20 * 3 * 2];
		int clothIndices[19 * 19 * 6];
		// float* clothVertices;
		// int* clothIndices;

		int meshResolution;
		float restLength[3];
		float mass;
		float K[3];

		std::vector<glm::vec3> vertexPosition;
		std::vector<glm::vec3> vertexNormal;
		std::vector<glm::vec3> vertexVelocity;
		
		void initMesh();
		void computeNormals();
		void simulate(float timeStep);

		glm::vec3 getForce(int i, int j);
		glm::vec3 getAllSprings(int i, int j);
		glm::vec3 getSpringForce(glm::vec3 p, glm::vec3 q, int type);
		glm::vec3 getGravityForce(int i, int j);
		glm::vec3 getDampingForce(int i, int j);
		glm::vec3 getViscousForce(int i, int j);

		glm::vec3 getPosition(int i, int j);
		glm::vec3 getNormal(int i, int j);
		glm::vec3 getVelocity(int i, int j);
		void setPosition(int i, int j, glm::vec3 value);
		void setVelocity(int i, int j, glm::vec3 value);

    public:
        Cloth(GLFWwindow* theWindow, glm::vec3 theLightPos, glm::vec3 theLightColor, float width, float height);
        void render(Camera* theCamera, int step);
		void clean();
};

#endif /* CLOTH_H */
