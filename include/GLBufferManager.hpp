//
//  GLBufferManager.hpp
//
//  Created by 吴博文 on 2018/5/21.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#ifndef GLBufferManager_hpp
#define GLBufferManager_hpp

#include <memory>
#include <numeric>
#include <vector>
#include "Cube.hpp"
#include "CubeManager.hpp"
#include "Global.hpp"
using std::shared_ptr;
using std::vector;

/**
 * This Class is used to Manager the data of VAO/VBO
 */
class GLBufferManager {
    friend class CubeManager;
    friend class Cube;

   public:
       static GLBufferManager& getInstance();

   private:
    // singleton
    GLBufferManager();
    ~GLBufferManager();

    // private data
    enum vboClass{POS=0, NORM, COLOR, MODEL};

    GLuint VAO;
    GLuint VBOs[4];

    vector<GLfloat> aCubeVertex;
    vector<GLfloat> aCubeNormal;
    vector<glm::vec4> colorVecs;
    vector<glm::mat4> modelMatrices;

    void updateCubeBufferData();
    void updateColorBufferData();
    void updateModelBufferData();

   public:
    // Singleton
    GLBufferManager(GLBufferManager const&) = delete;
    // void operator=(GLBufferManager const&) = delete;

    void init(const int _totalCubeNum, const int aCubeVertexSize, const int aCubeNormSize);

    void setCubeData(const vector<GLfloat>& _aCubeVertex,
                     const vector<GLfloat>& _aCubeNormal);
    void setAllColor(const vector<glm::vec4>& _colorVecs);
    void setAllModel(const vector<glm::mat4>& _modelMatrices);

    void setColor(const int _id, const glm::vec4 _aColor);
    void setModel(const int _id, const glm::mat4 _aModel);

    void bind();
    void unbind();

    void bufferData();
    
    bool isCubeDataNeedUpdate;
    bool isColorDataNeedUpdate;
    bool isModelDataNeedUpdate;
};
#endif