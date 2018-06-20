#include "GLBufferManager.hpp"

GLBufferManager::GLBufferManager() {
    /**
    * 单例模式
        * 分配缓冲空间
        */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &colorInstanceVBO);
    glGenBuffers(1, &modelInstanceVBO);
}

void GLBufferManager::bufferData() {
    GLBufferManager::updateCubeBufferData();
    GLBufferManager::updateColorBufferData();
    GLBufferManager::updateModelBufferData();
}

void GLBufferManager::updateCubeBufferData() {
    this->bind();
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, aCubeData.size() * sizeof(GLfloat),
                 &aCubeData.front(), GL_STATIC_DRAW);

    // 在这里set属性分布
    this->unbind();
}

void GLBufferManager::updateColorBufferData() {
    this->bind();
    glBindBuffer(GL_ARRAY_BUFFER, colorInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, colorVecs.size() * sizeof(glm::vec3),
                 &colorVecs.front(), GL_STATIC_DRAW);

    // 在这里set属性分布
    this->unbind();
}

void GLBufferManager::updateModelBufferData() {
    this->bind();

    glBindBuffer(GL_ARRAY_BUFFER, modelInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4),
                 &modelMatrices.front(), GL_STATIC_DRAW);

    // 在这里set属性分布
    this->unbind();
}

void GLBufferManager::init(const int _totalCubeNum) {
    aCubeData.resize(36 * 3 + 36 * 3);
    colorVecs.resize(_totalCubeNum);
    modelMatrices.resize(_totalCubeNum);
}

void GLBufferManager::bind() { glBindVertexArray(VAO); }

void GLBufferManager::unbind() { glBindVertexArray(0); }