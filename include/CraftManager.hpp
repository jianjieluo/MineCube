//
//  CraftManager.hpp
//
//  Created by 吴博文 on 2018/4/19.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#ifndef CraftManager_hpp
#define CraftManager_hpp

#include "Global.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <numeric>

using std::vector;
using std::string;


class CraftManager {
    private:
        static const unsigned int COOR_DIMENSION = 3;
        static const unsigned int TRIANGLE_PER_CUBE = 36;
        static const GLfloat cubeVertex[108];
        /**
         * 这个vector用以保存一个基本的小立方体的信息
         * 在后面通过对其进行变换得到多个立方体
         */
        vector<GLfloat> vertex;
        /**
         * 着色器中的model矩阵的名字
         */
        string modelMat4Name;
        unsigned int shaderID;

        unsigned int VAO;
        unsigned int VBO;

        unsigned int totalLengthPerVertex;
        /**
         * 构成大立方体的每个小cube的坐标(相对原点)
         */
        vector<glm::vec3> cubesPosition;
		void setModel(const glm::mat4 & mat4);

    public:
		CraftManager(
            GLfloat sizePerCube,
			unsigned int numPerEdge,
			unsigned int shaderID,
			string mat4Name,
			const vector<GLuint>& attriSize);

		~CraftManager();

		void draw();

		void setAttriArray(unsigned int offset, unsigned int size, const vector<GLfloat> & data);
};

#endif /* CraftManager_hpp */
