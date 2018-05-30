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
    public:
        CubeManager(unsigned int, unsigned int, unsigned int);
        ~CubeManager();
        
        shared_ptr<Cube> getCube(unsigned int, unsigned int, unsigned int);

        /**
         * If there already have one cube,
         * over-write it.
         */
        void setCube(unsigned int, unsigned int, unsigned int, shared_ptr<Cube>);

        /**
         * If there is no cube,
         * noting happen
         */
        void deleteCube(unsigned int, unsigned int, unsigned int);

        /**
         * Call this method inside render loop
         */
        void draw();

        void defalut_init_all();
};

#endif /* CubeManager_hpp */