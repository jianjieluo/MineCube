#include "GLBufferManager.hpp"
#include <assert.h>


GLBufferManager& GLBufferManager::getInstance() {
    static GLBufferManager instance;  // Guaranteed to be destroyed.
                                      // Instantiated on first use.
    return instance;
}


GLBufferManager::GLBufferManager() {
    /**
        * 分配缓冲空间
        */
    glGenVertexArrays(1, &VAO);
    glGenBuffers(4, VBOs);
}

GLBufferManager::~GLBufferManager() {
    /**
     * 销毁buffer
     */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(4, VBOs);
}

void GLBufferManager::init(const int _totalCubeNum, const int aCubeVertexSize, const int aCubeNormSize) {
    aCubeVertex.clear();
    aCubeVertex.resize(aCubeVertexSize);
    aCubeNormal.clear();
    aCubeNormal.resize(aCubeNormSize);
    
    colorVecs.clear();
    colorVecs.resize(_totalCubeNum);
    
    modelMatrices.clear();
    modelMatrices.resize(_totalCubeNum);
}

void GLBufferManager::updateCubeBufferData() {
    this->bind();

    // set Vertex Position
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[vboClass::POS]);
    glBufferData(GL_ARRAY_BUFFER, aCubeVertex.size() * sizeof(GLfloat), &aCubeVertex.front(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // set vertex Normal
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[vboClass::NORM]);
    glBufferData(GL_ARRAY_BUFFER, aCubeNormal.size() * sizeof(GLfloat), &aCubeNormal.front(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->unbind();
}

void GLBufferManager::updateColorBufferData() {
    this->bind();
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[vboClass::COLOR]);
    glBufferData(GL_ARRAY_BUFFER, colorVecs.size() * sizeof(glm::vec4),
                 &colorVecs.front(), GL_DYNAMIC_DRAW);

    // 在这里set属性分布
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
    // 设置 instance 的更新率
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->unbind();
}

void GLBufferManager::updateModelBufferData() {
    this->bind();

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[vboClass::MODEL]);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4),
                 &modelMatrices.front(), GL_DYNAMIC_DRAW);

    // 在这里set属性分布
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (void*)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size,
                          (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    this->unbind();
}

void GLBufferManager::bufferData() {
    if (this->isCubeDataNeedUpdate) {
        updateCubeBufferData();
        this->isCubeDataNeedUpdate = false;
    }
    if (this->isColorDataNeedUpdate) {
        updateColorBufferData();
        this->isColorDataNeedUpdate = false;
    }
    if (this->isModelDataNeedUpdate) {
        updateModelBufferData();
        this->isModelDataNeedUpdate = false;
    }
}

/* =================================================================

Set attri below, not update buffer

 =================================================================*/
void GLBufferManager::setCubeData(const vector<GLfloat>& _aCubeVertex,
                                  const vector<GLfloat>& _aCubeNormal) {
    assert(aCubeVertex.size() ==
           _aCubeVertex.size());  // The size of two data is not the same
    aCubeVertex.assign(_aCubeVertex.begin(), _aCubeVertex.end());

    assert(aCubeNormal.size() == _aCubeNormal.size());
    aCubeNormal.assign(_aCubeNormal.begin(), _aCubeNormal.end());

    this->isCubeDataNeedUpdate = true;
}

void GLBufferManager::setAllColor(const vector<glm::vec4>& _colorVecs) {
    assert(colorVecs.size() ==
           _colorVecs.size());  // The size of two data is not the same
    colorVecs.assign(_colorVecs.begin(), _colorVecs.end());

    if (!this->isColorDataNeedUpdate) {
        this->isColorDataNeedUpdate = true;
    }
    
}
void GLBufferManager::setAllModel(const vector<glm::mat4>& _modelMatrices) {
    assert(modelMatrices.size() ==
           _modelMatrices.size());  // The size of two data is not the same
    modelMatrices.assign(_modelMatrices.begin(), _modelMatrices.end());
    if (!this->isModelDataNeedUpdate) {
        this->isModelDataNeedUpdate = true;
    }
}

void GLBufferManager::setColor(const int _id, const glm::vec4 _aColor) {
    colorVecs[_id] = _aColor;
    if (!this->isColorDataNeedUpdate) {
        this->isColorDataNeedUpdate = true;
    }
}

void GLBufferManager::setModel(const int _id, const glm::mat4 _aModel) {
    modelMatrices[_id] = _aModel;
    if (!this->isModelDataNeedUpdate) {
        this->isModelDataNeedUpdate = true;
    }
}

void GLBufferManager::bind() { glBindVertexArray(VAO); }

void GLBufferManager::unbind() { glBindVertexArray(0); }
