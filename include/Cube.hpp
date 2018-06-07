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
        static const unsigned int COLOR_DIMENSION = 3;
        static const unsigned int VERTEX_PER_CUBE = 36;
        static const unsigned int VERTEX_PER_PLANE = 6;
        static const vector<GLfloat> cubeVertex;
        static const vector<GLfloat> cubeNormal;
        static const vector<GLfloat> initCubeColor;

        static const unsigned int POSITION_ATTRI_OFFSET = 0;
        static const unsigned int NOR_VECTOR_ATTRI_OFFSET = 3;
        static const unsigned int COLOR_ATTRI_OFFSET = 6;

        static const unsigned int LEFT_PLANE_BEGIN = 0;
        static const unsigned int RIGHT_PLANE_BEGIN = 0;
        static const unsigned int UP_PLANE_BEGIN = 0;
        static const unsigned int BOTTOM_PLANE_BEGIN = 0;
        static const unsigned int FRONT_PLANE_BEGIN = 0;
        static const unsigned int BACK_PLANE_BEGIN = 0;

        static const unsigned int LEFT = 0;
        static const unsigned int RIGHT = 1;
        static const unsigned int UP = 2;
        static const unsigned int BOTTOM = 3;
        static const unsigned int FRONT = 4;
        static const unsigned int BACK = 5;

        glm::mat4 model;
        string modelMat4Name;
        GLuint shaderID;
        shared_ptr<GLBufferManager> glBufferManager;
        vector<GLfloat> cubeColor;
        void useModelMat4();
        void beforeDraw();
        void afterDraw();
    public:
        Cube(
            GLfloat size,
            GLuint shaderID,
            string mat4Name,
            vector<GLuint> attriSize
        );
        Cube(const Cube &) = delete;
        ~Cube();

        virtual void drawLeft();
        virtual void drawRight();
        virtual void drawUp();
        virtual void drawBottom();
        virtual void drawFront();
        virtual void drawBack();
        virtual void drawAll();

        virtual void editColor(
            GLfloat r,
            GLfloat g,
            GLfloat b,
            unsigned int plane
        );

        virtual void setModelMat4(const glm::mat4 &);

        virtual void setShaderId(const GLuint & shaderID);
};



#endif /* Cube_hpp */
