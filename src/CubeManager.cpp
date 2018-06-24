#include "CubeManager.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stdexcept>
#include "Cube.hpp"
#include "GLBufferManager.hpp"
using std::cout;
using std::endl;
CubeManager::CubeManager(unsigned int width, unsigned int height,
                         unsigned int depth, GLfloat sizePerCube)
    : width(width),
      height(height),
      depth(depth),
      sizePerCube(sizePerCube),
      glBufferManager(GLBufferManager::getInstance()) {
    totalCube = width * height * depth;
    auto aCubeVertexDataSize = Cube::VERTEX_PER_CUBE * Cube::COOR_DIMENSION;
    auto aCubeNormDataSize = Cube::VERTEX_PER_CUBE * Cube::NOR_VECTOR_DIMENSION;
    this->glBufferManager.init(totalCube, aCubeVertexDataSize,
                               aCubeNormDataSize);

    // 唯一一次初始化cube的顶点属性
    vector<GLfloat> vertexTemp =
        vector<GLfloat>(Cube::VERTEX_PER_CUBE * Cube::COOR_DIMENSION);
    for (unsigned int i = 0; i < vertexTemp.size(); ++i) {
        vertexTemp[i] = sizePerCube * Cube::cubeVertex[i];
    }
    this->glBufferManager.setCubeData(vertexTemp, Cube::cubeNormal);

    // 初始化cubes数组
    cubes = vector<shared_ptr<Cube>>(totalCube);

    // generate set of cubes position
    cubesOriginalPosition = vector<glm::vec3>(totalCube);
    // 计算遍历的起始点
    GLfloat coorMinX = -1 * sizePerCube * width / 2 + sizePerCube / 2;
    GLfloat coorMinY = -1 * sizePerCube * height / 2 + sizePerCube / 2;
    GLfloat coorMinZ = -1 * sizePerCube * depth / 2 + sizePerCube / 2;
    glm::vec3 startPoint = glm::vec3(coorMinX, coorMinY, coorMinZ);
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int z = 0; z < depth; ++z) {
                auto id = getId(x, y, z);
                cubesOriginalPosition[id] =
                    glm::vec3(x * sizePerCube, y * sizePerCube,
                              z * sizePerCube) +
                    startPoint;
            }
        }
    }

    rotateAngleAroundY = 0;
    rotateAngleAroundX = 0;
}

CubeManager::~CubeManager() {
    // do nothing
}

shared_ptr<Cube> CubeManager::getCube(unsigned int x, unsigned int y,
                                      unsigned int z) {
    return cubes[getId(x, y, z)];
}

// set cube with vec4 color
void CubeManager::setCube(unsigned int x, unsigned int y, unsigned int z,
                          glm::vec4 color, const unsigned int shaderID) {
    unsigned int&& id = getId(x, y, z);
    auto newCube = shared_ptr<Cube>(new Cube(id, sizePerCube, shaderID));
    newCube->editColor(color.x, color.y, color.z, color.w);
    newCube->setModelMat4(calculateModelMat4(id));
    cubes[id] = newCube;
}

// set cube with vec3 color, alpha default is 1.0
void CubeManager::setCube(unsigned int x, unsigned int y, unsigned int z,
                          glm::vec3 color, const unsigned int shaderID) {
    setCube(x, y, z, glm::vec4(color, 1.0), shaderID);
}

bool CubeManager::isThereACube(unsigned int x, unsigned int y, unsigned int z) {
    unsigned int&& id = getId(x, y, z);
    return !this->cubes[id]->isDeleted();
}

void CubeManager::deleteCube(unsigned int x, unsigned int y, unsigned int z) {
    unsigned int&& id = getId(x, y, z);
    auto temp = cubesOriginalPosition[id];
    cubesOriginalPosition[id] = CubeManager::infPos;
    cubes[id]->setModelMat4(calculateModelMat4(id));
    cubes[id]->isInInf = true;
    cubesOriginalPosition[id] = temp;
}

/**
 * This may be a bottleneck of performance.
 * We should optimize here later.
 */
void CubeManager::draw() {
    this->glBufferManager.bufferData();
    this->glBufferManager.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, totalCube);
    this->glBufferManager.unbind();
}

unsigned int CubeManager::getId(unsigned int x, unsigned int y,
                                unsigned int z) {
    // check arguments
    if (x >= width || y >= height || z >= depth) {
        throw std::out_of_range(
            "OUT_OF_RANGE:Please make input argument is in legal range.");
    }
    return x * (height * depth) + y * depth + z;
}

unsigned int CubeManager::getId(unsigned int x, unsigned int y, unsigned int z,
                                unsigned int w) {
    if (x >= width || y >= height || z >= depth || w >= 6) {
        throw std::out_of_range(
            "OUT_OF_RANGE:Please make input argument is in legal range.");
    }
    return x * (height * depth * 6) + y * (depth * 6) + z * 6 + w;
}

// generate default cube for each position
void CubeManager::defalut_init_all(const GLuint& shaderID,
                                   const string& mat4Name) {
    for (unsigned int i = 0; i < totalCube; ++i) {
        cubes[i] = shared_ptr<Cube>(new Cube(i, sizePerCube, shaderID));
        cubes[i]->cubeColor2Buffer();
    }
    refreshModelMat4();
}

void CubeManager::refreshModelMat4() {
    for (unsigned int i = 0; i < totalCube; ++i) {
        if (!cubes[i]->isDeleted()) {
            cubes[i]->setModelMat4(calculateModelMat4(i));
        }
    }
}

void CubeManager::refreshModelMat4(const unsigned int& id) {
    if (cubes[id] == nullptr) {
        throw std::invalid_argument(
            "INVALID:There is no cube in this position!");
    }
    cubes[id]->setModelMat4(calculateModelMat4(id));
}

glm::mat4 CubeManager::calculateModelMat4(const unsigned int& id) {
    glm::mat4 model;
    model = glm::rotate(model, glm::radians(rotateAngleAroundY), yAxis);
    model = glm::rotate(model, glm::radians(rotateAngleAroundX), xAxis);
    return glm::translate(model, cubesOriginalPosition[id]);
}

void CubeManager::setAllShaderId(const GLuint& shaderID) {
    for (unsigned int i = 0; i < totalCube; ++i) {
        if (cubes[i] != nullptr) {
            cubes[i]->setShaderId(shaderID);
        }
    }
}

void CubeManager::rotateVertical(const GLfloat& offset) {
    rotateAngleAroundX += offset * rotateSensivitiy;
    this->refreshModelMat4();
}

void CubeManager::rotateHorizontal(const GLfloat& offset) {
    rotateAngleAroundY += offset * rotateSensivitiy;
    this->refreshModelMat4();
}

void CubeManager::setRotateSensivity(const GLfloat& rotateSensivitiy) {
    this->rotateSensivitiy = rotateSensivitiy;
}

glm::mat4 CubeManager::getModelMat4(unsigned int x, unsigned int y,
                                    unsigned int z) {
    return calculateModelMat4(this->getId(x, y, z));
}
const glm::vec3 CubeManager::yAxis = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CubeManager::xAxis = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 CubeManager::infPos = glm::vec3(3000.0f, 3000.0f, 3000.0f);