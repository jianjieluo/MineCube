#include "CubeManager.hpp"
#include <stdexcept>

CubeManager::CubeManager(
    unsigned int width,
    unsigned int height,
    unsigned int depth,
    GLfloat sizePerCube
):width(width), height(height), depth(depth), sizePerCube(sizePerCube) {
    // generate set of cubes position
    totalCube = width * height * depth;
    cubesPosition = vector<glm::vec3>(totalCube);
    // 计算遍历的起始点
    GLfloat coorMinX = -1 * sizePerCube * width  / 2 + sizePerCube / 2;
    GLfloat coorMinY = -1 * sizePerCube * height / 2 + sizePerCube / 2;
    GLfloat coorMinZ = -1 * sizePerCube * depth  / 2 + sizePerCube / 2;
    glm::vec3 startPoint = glm::vec3(coorMinX, coorMinY, coorMinZ);
    for (unsigned int x = 0; x < width; ++x) {
        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int z = 0; z < depth; ++z) {
                cubesPosition[getId(x, y, z)] = glm::vec3(x * sizePerCube, y * sizePerCube, z * sizePerCube + startPoint);
            }
        }
    }

    // 初始化cubes数组
    cubes = vector<shared_ptr<Cube>>(totalCube);
}

CubeManager::~CubeManager() {
    // do nothing
}

shared_ptr<Cube> CubeManager::getCube(
    unsigned int x,
    unsigned int y,
    unsigned int z
) {
    return cubes[getId(x, y, z)];
}

void CubeManager::setCube(
    unsigned int x,
    unsigned int y,
    unsigned int z,
    shared_ptr<Cube> newCube
) {
    cubes[getId(x, y, z)] = newCube;
}

void CubeManager::deleteCube(
    unsigned int x,
    unsigned int y,
    unsigned int z
) {
    cubes[getId(x, y, z)] = nullptr;
}

/**
 * This may be a bottleneck of performance.
 * We should optimize here later.
 */
void CubeManager::draw() {
    for (unsigned int i = 0; i < totalCube; ++i) {
        if (cubes[i] != nullptr) {
            cubes[i]->drawAll();
        }
    }
}

unsigned int CubeManager::getId(
    unsigned int x,
    unsigned int y,
    unsigned int z
) {
    // check arguments
    if (x >= width || y >= height || z >= depth) {
        throw std::out_of_range("OUT_OF_RANGE:Please make input argument is in legal range.");
    }
    return x * (height + depth) + y * depth + z;
}

unsigned int CubeManager::getId(
    unsigned int x,
    unsigned int y,
    unsigned int z,
    unsigned int w
) {
    if (x >= width || y >= height || z >= depth || w >= 6) {
        throw std::out_of_range("OUT_OF_RANGE:Please make input argument is in legal range.");
    }
    return x * (height + depth + 6) + y * (depth + 6) + z * 6 + w;
}

