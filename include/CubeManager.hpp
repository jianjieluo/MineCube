//
//  CubeManager.hpp
//
//  Created by 吴博文 on 2018/5/22.
//  Modified by 罗剑杰 on 2018/6/23

//  Use instances array to manage the voxel cube

#ifndef CubeManager_hpp
#define CubeManager_hpp

#include "Global.hpp"
#include "Cube.hpp"
#include "GLBufferManager.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

/**
 * CubeManager aim to manage a set of cubes
 */
class CubeManager {
    private:

        static const glm::vec3 yAxis;
        static const glm::vec3 xAxis;
        unsigned int width; // x
        unsigned int height; // y
        unsigned int depth; // z
        unsigned int totalCube;
        GLfloat sizePerCube;


        // instance array related
        shared_ptr<GLBufferManager> glBufferManager;

        vector<glm::vec3> cubesPosition;
        vector<shared_ptr<Cube>> cubes;

        vector<glm::mat4> modelMatrices;
        vector<glm::vec3> colorVecs;


        // glm::vec3 rotateAngle;
        GLfloat rotateAngleAroundX;
        GLfloat rotateAngleAroundY;
        GLfloat rotateSensivitiy = 1.0f;
        void refreshModelMat4();
        void refreshModelMat4(const unsigned int & id);
        glm::mat4 calculateModelMat4(const unsigned int & id);

        void beforeDraw();
        void afterDraw();


    public:
        CubeManager(
            unsigned int width,
            unsigned int height,
            unsigned int depth,
            GLfloat sizePerCube
        );
        // restore from file
        CubeManager(string model_path) = delete;
        // store to file
        // void dump(string model_path);
        ~CubeManager();
        
        shared_ptr<Cube> getCube(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );

        /**
         * If there already have one cube,
         * over-write it.
         * The incoming cube's size can be any value, just
         * it may cover other cubes.
         */
        void setCube(
            unsigned int x,
            unsigned int y,
            unsigned int z,
            shared_ptr<Cube> newCube
        );

        /**
         * If there is no cube,
         * noting happen
         */
        void deleteCube(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );

        /**
         * Call this method inside render loop
         */
        void draw();

        void defalut_init_all(const GLuint & shaderID, const string & mat4Name);

        unsigned int getId(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );

        unsigned int getId(
            unsigned int x,
            unsigned int y,
            unsigned int z,
            unsigned int w
        );

        /**
         * Change all shaderID of cubes managed by this CubeManager
         * This is not a cheap operation.
         * Warning: Calling this function will change ALL CUBE shader!
         * If you just modify part of cubes' shader, use Cube::setShaderId instead.
         */
        void setAllShaderId(const GLuint & shaderID);

        void rotateHorizontal(const GLfloat & offset);

        void rotateVertical(const GLfloat & offset);

        void setRotateSensivity(const GLfloat & rotateSensivitiy);

        glm::mat4 getModelMat4(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );
};

#endif /* CubeManager_hpp */