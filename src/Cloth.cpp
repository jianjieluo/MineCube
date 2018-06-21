#include "Cloth.hpp"


Cloth::Cloth(GLFWwindow* theWindow, glm::vec3 theLightPos, glm::vec3 theLightColor, float width, float height) {
	std::cout << "init cloth" << std::endl;

	window = theWindow;
	lightPos = theLightPos;
	lightColor = theLightColor;
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	clothShader = new Shader("../src/Shader/cloth.vs", "../src/Shader/cloth.fs");

	meshResolution = 20;
	mass = 1.0;
	restLength[0] = 4.0 / static_cast<float>(meshResolution - 1);
	restLength[1] = sqrt(2.0) * 4.0 / static_cast<float>(meshResolution - 1);
	restLength[2] = 2.0 * restLength[0];
	K[0] = K[1] = K[2] = 25000.0;
	// clothVertices = new float[meshResolution * meshResolution * 3 * 2];
	// clothIndices = new int[(meshResolution - 1) * (meshResolution - 1) * 6];
	initMesh();
}


void Cloth::render(Camera* camera, int step) {
    // code
	std::cout << "cloth simulate" << std::endl;
	float timeStep = 0.001;
	int n = static_cast<int>(ceil(0.01 / timeStep));
	for (int i = 0; i < n; i++) {
		simulate(timeStep);
	}
	computeNormals();

	// updateBuffers
	for (int i = 0; i < meshResolution; i++) {
		for (int j = 0; j < meshResolution; j++) {
			int id = i * meshResolution + j;
			glm::vec3 position = getPosition(i, j);
			clothVertices[id * 6] = position.x;
			clothVertices[id * 6 + 1] = position.y;
			clothVertices[id * 6 + 2] = position.z;
			glm::vec3 normal = getNormal(i, j);
			clothVertices[id * 6 + 3] = normal.x;
			clothVertices[id * 6 + 4] = normal.y;
			clothVertices[id * 6 + 5] = normal.z;
		}
	}

	// glBufferData(GL_ARRAY_BUFFER, sizeof(clothVertices), clothVertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &clothVAO);
	glGenBuffers(1, &clothVBO);
	glGenBuffers(1, &clothEBO);

	glBindBuffer(GL_ARRAY_BUFFER, clothVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clothVertices), clothVertices, GL_STATIC_DRAW);

	glBindVertexArray(clothVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clothEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(clothIndices), clothIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::vec3 specular(0.2f, 0.2f, 0.2f);
	float shininess = 32.0f;

	clothShader->use();
	clothShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	clothShader->setVec3("lightColor", lightColor);
	clothShader->setVec3("lightPos", lightPos);
	clothShader->setVec3("viewPos", camera->getCameraPosition());

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera->getZoomFactor()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
	clothShader->setMat4("projection", projection);
	clothShader->setMat4("view", view);

	// world transformation
	glm::mat4 model;
	float curPos = 0.0f + step * timeStep * 3.0 > 1.0f ? 1.0f : 0.0f + step * timeStep * 3.0;
	glm::vec3 newPos(0.0f, curPos, -2.5f);  // 0.7f

	model = glm::translate(model, newPos);
	model = glm::scale(model, glm::vec3(0.22f));
	clothShader->setMat4("model", model);

	glBindVertexArray(clothVAO);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, (meshResolution-1) * (meshResolution-1) * 6, GL_UNSIGNED_INT, 0);

	glDeleteVertexArrays(1, &clothVAO);
	glDeleteBuffers(1, &clothVBO);
	glDeleteBuffers(1, &clothEBO);
}

void Cloth::clean() {
	// code
}

void Cloth::initMesh() {
	// code
	std::cout << "build mesh" << std::endl;

	for (int i = 0; i < meshResolution; i++) {
		for (int j = 0; j < meshResolution; j++) {
			glm::vec3 initPosition(-2.0 + 4.0*j / static_cast<float>(meshResolution - 1), -2.0 + 4.0*i / static_cast<float>(meshResolution - 1), 0.0);
			glm::vec3 initVelocity(0.0f, 0.0f, 0.0f);
			glm::vec3 initNormal(0.0f, 0.0f, 0.0f);
			vertexPosition.push_back(initPosition);
			vertexVelocity.push_back(initVelocity);
			vertexNormal.push_back(initNormal);
		}
	}
	computeNormals();
	int k = 0;
	for (int i = 0; i < meshResolution - 1; i++) {
		for (int j = 0; j < meshResolution - 1; j++) {
			clothIndices[6 * k] = i * meshResolution + j;
			clothIndices[6 * k + 1] = i * meshResolution + j + 1;
			clothIndices[6 * k + 2] = (i + 1) * meshResolution + j + 1;
			clothIndices[6 * k + 3] = i * meshResolution + j;
			clothIndices[6 * k + 4] = (i + 1) * meshResolution + j + 1;
			clothIndices[6 * k + 5] = (i + 1) * meshResolution + j;
			++k;
		}
	}
}

glm::vec3 Cloth::getPosition(int i, int j) {
	int index = i * meshResolution + j;
	return glm::vec3(vertexPosition[index].x, vertexPosition[index].y, vertexPosition[index].z);
}

void Cloth::setPosition(int i, int j, glm::vec3 value) {
	int index = i * meshResolution + j;
	vertexPosition[index] = value;
}

glm::vec3 Cloth::getNormal(int i, int j) {
	int index = i * meshResolution + j;
	return glm::vec3(vertexNormal[index].x, vertexNormal[index].y, vertexNormal[index].z);
}

glm::vec3 Cloth::getVelocity(int i, int j) {
	int index = i * meshResolution + j;
	return glm::vec3(vertexVelocity[index].x, vertexVelocity[index].y, vertexVelocity[index].z);
}

void Cloth::setVelocity(int i, int j, glm::vec3 value) {
	int index = i * meshResolution + j;
	vertexVelocity[index] = value;
}

void Cloth::computeNormals() {
	std::cout << "compute normals" << std::endl;
	int dx[6] = { 1, 1, 0, -1, -1, 0 }, dy[6] = { 0, 1, 1, 0, -1, -1 };
	glm::vec3 e1, e2;
	int k = 0;
	for (int i = 0; i < meshResolution; i++) {
		for (int j = 0; j < meshResolution; j++) {
			glm::vec3 p0 = getPosition(i, j);
			std::vector<glm::vec3> norms;
			for (int t = 0; t < 6; t++) {
				int i1 = i + dy[t], j1 = j + dx[t];
				int i2 = i + dy[(t + 1) % 6], j2 = j + dx[(t + 1) % 6];
				if (i1 >= 0 && i1 < meshResolution && j1 >= 0 && j1 < meshResolution &&
					i2 >= 0 && i2 < meshResolution && j2 >= 0 && j2 < meshResolution) {
					e1 = getPosition(i1, j1) - p0;
					e2 = getPosition(i2, j2) - p0;
					norms.push_back(glm::normalize(glm::cross(e1, e2)));
				}
			}
			e1 = glm::vec3(0.0f, 0.0f, 0.0f);
			for (int t = 0; t < norms.size(); t++) {
				e1 = e1 + norms[t];
			}
			int index = i * meshResolution + j;
			vertexNormal[index] = glm::normalize(e1);
		}
	}
}

void Cloth::simulate(float stepSize) {
	// code
	glm::vec3 pin1 = getPosition(meshResolution - 1, 0), pin2 = getPosition(meshResolution - 1, meshResolution - 1);

	for (int i = 0; i < meshResolution; i++) {
		for (int j = 0; j < meshResolution; j++) {
			glm::vec3 newVelocity = getVelocity(i, j) + getForce(i, j) * stepSize / mass;
			setVelocity(i, j, newVelocity);
		}
	}

	// Notice that the updated velocity above is used for better numerical stability.
	for (int i = 0; i < meshResolution; i++) {
		for (int j = 0; j < meshResolution; j++) {
			glm::vec3 newPosition = getPosition(i, j) + getVelocity(i, j) * stepSize;
			setPosition(i, j, newPosition);
		}
	}

	setPosition(meshResolution - 1, 0, pin1);
	setPosition(meshResolution - 1, meshResolution - 1, pin2);
}

glm::vec3 Cloth::getForce(int i, int j) {
	glm::vec3 F_spring = getAllSprings(i, j) + getGravityForce(i, j) + getDampingForce(i, j) + getViscousForce(i, j);
	return F_spring;
}

glm::vec3 Cloth::getAllSprings(int i, int j) {
	glm::vec3 f = glm::vec3(0.0f, 0.0f, 0.0f);

	// 0.Structural: [i, j+1], [i, j-1], [i+1, j], [i-1, j]
	if ((j + 1) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i, j + 1), 0);
	}
	if ((j - 1) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i, j - 1), 0);
	}
	if ((i + 1) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i + 1, j), 0);
	}
	if ((i - 1) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i - 1, j), 0);
	}

	// 1.Shear: [i+1, j+1], [i+1, j-1], [i-1, j-1], [i-1, j+1]
	if ((i + 1) < meshResolution && (j + 1) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i + 1, j + 1), 1);
	}
	if ((i + 1) < meshResolution && (j - 1) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i + 1, j - 1), 1);
	}
	if ((i - 1) >= 0 && (j - 1) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i - 1, j - 1), 1);
	}
	if ((i - 1) >= 0 && (j + 1) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i - 1, j + 1), 1);
	}

	// 2.Flexion: [i, j+2], [i, j-2], [i+2, j], [i-2, j]
	if ((j + 2) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i, j + 2), 2);
	}
	if ((j - 2) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i, j - 2), 2);
	}
	if ((i + 2) < meshResolution) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i + 2, j), 2);
	}
	if ((i - 2) >= 0) {
		f = f + getSpringForce(getPosition(i, j), getPosition(i - 2, j), 2);
	}
	return f;
}
glm::vec3 Cloth::getSpringForce(glm::vec3 p, glm::vec3 q, int type) {
	glm::vec3 p_q = p - q;
	float len = glm::length(p_q);
	glm::vec3 result = p_q * (K[type] * (restLength[type] - len) / len);
	return result;
}
glm::vec3 Cloth::getGravityForce(int i, int j) {
	float g = 9.8;
	return glm::vec3(0.0f, - mass * g, 0.0f);
}
glm::vec3 Cloth::getDampingForce(int i, int j) {
	float Cd = 0.5;
	return getVelocity(i, j) * (-Cd);
}
glm::vec3 Cloth::getViscousForce(int i, int j) {
	float Cv = 0.5;
	glm::vec3 uf = glm::vec3(0.0f, 0.0f, 1.0f);
	float factor = Cv * glm::dot(getNormal(i, j), (uf - getVelocity(i, j)));
	return getNormal(i, j) * factor;
}