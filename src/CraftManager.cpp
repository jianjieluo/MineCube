//
//  CraftManager.cpp
//  TestForOpenGL
//
//  Created by 吴博文 on 2018/4/19.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#include "CraftManager.hpp"


const GLfloat CraftManager::cubeVertex[108] = {
    1.0f, -1.0f, -1.0f, 
    -1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f, 
    1.0f,  1.0f, -1.0f, 
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f, 
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f, 
    1.0f, -1.0f,  1.0f, 
    1.0f, -1.0f,  1.0f, 
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f
};


CraftManager::CraftManager(GLfloat sizePerCube,
	unsigned int numPerEdge,
	unsigned int shaderID,
	string mat4Name,
	const vector<GLuint>& attriSize) :modelMat4Name(mat4Name), shaderID(shaderID) {
	/**
	* 分配缓冲空间
	*/
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	totalLengthPerVertex = COOR_DIMENSION + std::accumulate(attriSize.begin(), attriSize.end(), 0);

	/**
	* 初始化vertex vector，每个小立方体需要36个点
	*/
	vertex = vector<GLfloat>(totalLengthPerVertex * TRIANGLE_PER_CUBE);
	// 初始化赋值，只考虑每个点的位置
	for (unsigned int i = 0; i < TRIANGLE_PER_CUBE; ++i) {
		unsigned int vertexInfoStartIndex = i * totalLengthPerVertex;
		unsigned int cubeVertexStartIndex = i * COOR_DIMENSION;
		for (int j = 0; j < COOR_DIMENSION; ++j) {
			vertex[vertexInfoStartIndex + j] = cubeVertex[cubeVertexStartIndex + j] * sizePerCube;
		}
	}
	GLfloat* bufferTemp = new GLfloat[vertex.size()];
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex.front(), GL_STATIC_DRAW);
	// 三维的位置信息，为必选项
	glVertexAttribPointer(0, COOR_DIMENSION, GL_FLOAT, GL_FALSE, totalLengthPerVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 其他的属性，optional
	GLuint offset = COOR_DIMENSION;
	for (GLuint i = 1; i <= attriSize.size(); ++i) {
		glVertexAttribPointer(i, attriSize[i - 1], GL_FLOAT, GL_FALSE,
			totalLengthPerVertex * sizeof(GLfloat),
			(void*)(offset * sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
		offset += attriSize[i - 1];
	}


	/**
	* 根据numPerEdge 初始化多个小立方体的位置
	*/
	cubesPosition = vector<glm::vec3>();
	// 计算遍历的起始点
	GLfloat coorMin = -1 * sizePerCube * numPerEdge / 2 + sizePerCube / 2;
	glm::vec3 startPoint = glm::vec3(coorMin, coorMin, coorMin);
	for (unsigned int x = 0; x < numPerEdge; ++x) {
		for (unsigned int y = 0; y < numPerEdge; ++y) {
			for (unsigned int z = 0; z < numPerEdge; ++z) {
				cubesPosition.push_back(glm::vec3(x * sizePerCube, y * sizePerCube, z * sizePerCube) + startPoint);
			}
		}
	}

	glBindVertexArray(0);
}
CraftManager::~CraftManager() {
	/**
	* 销毁buffer
	*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
}

void CraftManager::draw() {
	glBindVertexArray(VAO);
	for (unsigned int i = 0; i < cubesPosition.size(); ++i) {
		glm::mat4 model;
		model = glm::translate(model, cubesPosition[i]);
		setModel(model);
		glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_PER_CUBE);
	}
}

void CraftManager::setAttriArray(unsigned int offset, unsigned int size, const vector<GLfloat> & data) {
	offset += COOR_DIMENSION;
	for (unsigned int i = 0; i < TRIANGLE_PER_CUBE; ++i) {
		unsigned int startIndex = i * totalLengthPerVertex;
		unsigned int dataIndex = i * size;
		for (unsigned int j = 0; j < size; ++j) {
			vertex[startIndex + j + offset] = data[dataIndex + j];
		}
	}
	// update
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex.front(), GL_STATIC_DRAW);
}

void CraftManager::setModel(const glm::mat4 & mat4) {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, modelMat4Name.c_str()), 1,
		GL_FALSE, glm::value_ptr(mat4));
}