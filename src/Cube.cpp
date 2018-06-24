#include "Cube.hpp"
#include "GLBufferManager.hpp"

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

Cube::Cube(unsigned int _id, GLfloat size, GLuint shaderID)
    : id(_id),
      shaderID(shaderID),
      glBufferManager(GLBufferManager::getInstance()),
      isInInf(false) {
    // variable `size` is useless here, because the initializaton is in
    // CubeManager's construction

    // make one cube has its own color
    cubeColor = initCubeColor;
}

Cube::Cube(GLfloat size, GLuint shaderID, const string& mat4Name,
           vector<GLuint> attriSize)
    : id(-1),
      shaderID(shaderID),
      glBufferManager(GLBufferManager::getInstance()) {
    vector<GLfloat> vertexTemp =
        vector<GLfloat>(VERTEX_PER_CUBE * COOR_DIMENSION);
    for (unsigned int i = 0; i < vertexTemp.size(); ++i) {
        vertexTemp[i] = size * Cube::cubeVertex[i];
    }
    glBufferManager.setCubeData(vertexTemp, Cube::cubeNormal);

    // make one cube has its own color
    cubeColor = initCubeColor;
}

Cube::~Cube() {
    // Nothing to do
}

bool Cube::isDeleted() const { return this->isInInf; }

void Cube::editColor(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha) {
    cubeColor = glm::vec4(r, g, b, alpha);
    if (id != -1) {
        cubeColor2Buffer();
    }
}

void Cube::hitted() {
    this->glBufferManager.setColor(this->id, Cube::hitColor);
}

void Cube::unhitted() {
    this->cubeColor2Buffer();
}

void Cube::selected() {
    this->glBufferManager.setColor(this->id, glm::vec4(cubeColor[0], cubeColor[1], cubeColor[2], cubeColor[3] - 0.5));    
}

void Cube::unselected() {
    this->cubeColor2Buffer();
}

void Cube::setId(const unsigned int _id) { this->id = _id; }

glm::vec4 Cube::getColor() { return cubeColor; }

glm::vec4 Cube::getColorOfPLane(unsigned int plane) { return cubeColor; }

void Cube::setModelMat4(const glm::mat4& model) {
    this->glBufferManager.setModel(this->id, model);
}

void Cube::cubeColor2Buffer() { glBufferManager.setColor(this->id, cubeColor); }

void Cube::setShaderId(const GLuint& shaderID) { this->shaderID = shaderID; }

// 36 triangles make up a cube
const vector<GLfloat> Cube::cubeVertex = {
    0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
    0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

    -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
    0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
    0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f};

// Normal vector for a cube
const vector<GLfloat> Cube::cubeNormal = {
    0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
    0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,

    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,

    -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
    -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
    0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,

    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f};

const glm::vec4 Cube::initCubeColor = glm::vec4(0.76f, 0.78f, 0.96f, 1.0f);
const glm::vec4 Cube::hitColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);