//
//  CubeManager.hpp
//
//  Created by 吴博文 on 2018/5/22.
//  Copyright © 2018年 吴博文. All rights reserved.
//

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
        unsigned int width; // x
        unsigned int height; // y
        unsigned int depth; // z
        unsigned int totalCube;
        GLfloat sizePerCube;
        vector<glm::vec3> cubesPosition;
        vector<shared_ptr<Cube>> cubes;
        glm::vec3 rotateAngle;
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

        void refreshModelMat4();
        void refreshModelMat4(const unsigned int & id);
        glm::mat4 calculateModelMat4(const unsigned int & id);
};

#endif /* CubeManager_hpp */