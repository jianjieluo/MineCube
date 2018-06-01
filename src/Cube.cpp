#include "Cube.hpp"

/**
 * Usage of static const variable is weird here.
 * I don't konw why.
 * By Bowen Wu.
 * https://stackoverflow.com/questions/4891067/weird-undefined-symbols-of-static-constants-inside-a-struct-class
 */
const unsigned int Cube::COOR_DIMENSION;
const unsigned int Cube::NOR_VECTOR_DIMENSION;
const unsigned int Cube::COLOR_DIMENSION;
const unsigned int Cube::VERTEX_PER_CUBE;
const unsigned int Cube::VERTEX_PER_PLANE;
Cube::Cube(
    GLfloat size,
    GLuint shaderID,
    string mat4Name,
    vector<GLuint> attrisize
):shaderID(shaderID), modelMat4Name(mat4Name) {
   /**
    * Preprocess attrisize
    * vertex coordinate and normal vector are default attribute
    */
//    attrisize.insert(attrisize.begin(), COLOR_DIMENSION);
   attrisize.insert(attrisize.begin(), NOR_VECTOR_DIMENSION);
   attrisize.insert(attrisize.begin(), COOR_DIMENSION);
   glBufferManager = GLBufferManager::getNewInstance(attrisize, VERTEX_PER_CUBE);
   vector<GLfloat> vertexTemp = vector<GLfloat>(VERTEX_PER_CUBE * COOR_DIMENSION);
   for (unsigned int i = 0; i < vertexTemp.size(); ++i) {
       vertexTemp[i] = size * Cube::cubeVertex[i];
   }
   // Position attri
   glBufferManager->setAttriArray(POSITION_ATTRI_OFFSET, COOR_DIMENSION, vertexTemp);
   // Normal Vector attri
   glBufferManager->setAttriArray(NOR_VECTOR_ATTRI_OFFSET, NOR_VECTOR_DIMENSION, Cube::cubeNormal);
   // Color attri
//    glBufferManager->setAttriArray(COLOR_ATTRI_OFFSET, COLOR_DIMENSION, initCubeColor);

   // make one cube has its own color
   cubeColor = initCubeColor;
}

Cube::~Cube() {
    // Nothing to do
}

void Cube::drawLeft() {
    this->beforeDraw();    
    glDrawArrays(GL_TRIANGLES, LEFT_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();
}

void Cube::drawRight() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, RIGHT_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();
}

void Cube::drawUp() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, UP_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();    
}

void Cube::drawBottom() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, BOTTOM_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();
}

void Cube::drawFront() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, FRONT_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();
}

void Cube::drawBack() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, BACK_PLANE_BEGIN, VERTEX_PER_PLANE);
    this->afterDraw();
}

void Cube::drawAll() {
    this->beforeDraw();
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_CUBE);
    this->afterDraw();
}

void Cube::beforeDraw() {
    glBufferManager->bind();    
    this->useModelMat4();    
    // glUseProgram(shaderID);
}

void Cube::afterDraw() {
    glBufferManager->unbind();
}

void Cube::editColor(
    GLfloat r,
    GLfloat g,
    GLfloat b,
    unsigned int plane
) {
    // calculate total offset first
    unsigned int totalOffset = plane * COLOR_DIMENSION * VERTEX_PER_PLANE;
    for (unsigned int i = 0; i < VERTEX_PER_PLANE; ++i) {
        unsigned int startIndex = i * COLOR_DIMENSION + totalOffset;
        cubeColor[startIndex] = r;
        cubeColor[startIndex + 1] = g;
        cubeColor[startIndex + 2] = b;
    }
    glBufferManager->setAttriArray(COLOR_ATTRI_OFFSET, COLOR_DIMENSION, cubeColor);
}

void Cube::setModelMat4(const glm::mat4 & model) {
    this->model = model;
}

void Cube::useModelMat4() {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, modelMat4Name.c_str()), 1,
		GL_FALSE, glm::value_ptr(model));
}
// 36 triangles make up a cube
const vector<GLfloat> Cube::cubeVertex = {
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

// Normal vector for a cube
const vector<GLfloat> Cube::cubeNormal = {
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

const vector<GLfloat> Cube::initCubeColor = {
        0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,

		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,
		0.0f,  0.0f, 1.0f,

		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,
		1.0f, 0.0f,  0.0f,

		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  0.0f,

		0.0f, 1.0f,  1.0f,
		0.0f, 1.0f,  1.0f,
		0.0f, 1.0f,  1.0f,
		0.0f, 1.0f,  1.0f,
		0.0f, 1.0f,  1.0f,
		0.0f, 1.0f,  1.0f,

		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f,
		1.0f,  0.0f,  1.0f
};