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
    public:
        CubeManager(
            unsigned int width,
            unsigned int height,
            unsigned int depth,
            GLfloat sizePerCube
        );
        
        ~CubeManager();
        
        shared_ptr<Cube> getCube(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );

        /**
         * If there already have one cube,
         * over-write it.
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

        void defalut_init_all();

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
};

#endif /* CubeManager_hpp */