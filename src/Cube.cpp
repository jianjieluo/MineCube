#include "Cube.hpp"

Cube::Cube(
    GLfloat size,
    GLuint shaderID,
    string mat4Name,
    vector<GLuint> attrisize
):shaderID(shaderID), modelMat4Name(mat4Name){
   /**
    * Preprocess attrisize
    * vertex coordinate and normal vector are default attribute
    */
   attrisize.insert(attrisize.begin(), NOR_VECTOR_DIMENSION);
   attrisize.insert(attrisize.begin(), COOR_DIMENSION);
   glBufferManager = GLBufferManager::getNewInstance(attrisize, TRIANGLE_PER_CUBE);
   vector<GLfloat> vertexTemp = vector<GLfloat>(TRIANGLE_PER_CUBE * 3);
   for (unsigned int i = 0; i < vertexTemp.size(); ++i) {
       vertexTemp[i] = size * Cube::cubeVertex[i];
   }
   glBufferManager->setAttriArray(0, 3, vertexTemp);
   glBufferManager->setAttriArray(3, 3, Cube::cubeNormal);
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