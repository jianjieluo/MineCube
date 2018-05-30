#include "Cube.hpp"

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
   attrisize.insert(attrisize.begin(), NOR_VECTOR_DIMENSION);
   attrisize.insert(attrisize.begin(), COOR_DIMENSION);
   glBufferManager = GLBufferManager::getNewInstance(attrisize, VERTEX_PER_CUBE);
   vector<GLfloat> vertexTemp = vector<GLfloat>(VERTEX_PER_CUBE);
   for (unsigned int i = 0; i < vertexTemp.size(); ++i) {
       vertexTemp[i] = size * Cube::cubeVertex[i];
   }
   glBufferManager->setAttriArray(0, 3, vertexTemp);
   glBufferManager->setAttriArray(3, 3, Cube::cubeNormal);
}

Cube::~Cube() {
    // Nothing to do
}

void Cube::drawLeft() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, LEFT_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();
}

void Cube::drawRight() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, RIGHT_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();    
}

void Cube::drawUp() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, UP_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();    
}

void Cube::drawBottom() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, BOTTOM_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();
}

void Cube::drawFront() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, FRONT_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();
}

void Cube::drawBack() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, BACK_PLANE_BEGIN, VERTEX_PER_PLANE);
    glBufferManager->unbind();
}

void Cube::drawAll() {
    glBufferManager->bind();
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_CUBE);
    glBufferManager->unbind();
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