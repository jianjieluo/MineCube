//
//  Cube.hpp
//
//  Created by 吴博文 on 2018/5/21.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#include "Global.hpp"
#include "GLBufferManager.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <numeric>
#include <memory>
using std::shared_ptr;
using std::vector;
using std::string;

/**
 * This is a base class for cube.
 * You can inherit it and create your own cube.
 */
class Cube {
    private:
        static const unsigned int COOR_DIMENSION = 3;
        static const unsigned int NOR_VECTOR_DIMENSION = 3;
        static const unsigned int TRIANGLE_PER_CUBE = 36;
        static const vector<GLfloat> cubeVertex;
        static const vector<GLfloat> cubeNormal;
    protected:

        glm::mat4 model;
        string modelMat4Name;
        GLuint shaderID;
        shared_ptr<GLBufferManager> glBufferManager;
    public:
        Cube(
            GLfloat size,
            GLuint shaderID,
            string mat4Name,
            vector<GLuint> attriSize
        );
        Cube(const Cube &);        
        ~Cube();

        virtual void drawLeft();
        virtual void drawRight();
        virtual void drawUp();
        virtual void drawBottom();
        virtual void drawFront();
        virtual void drawBack();
        virtual void drawAll();
};



#endif /* Cube_hpp */
