//
//  CraftManager.hpp
//
//  Created by 吴博文 on 2018/4/19.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#ifndef CraftManager_hpp
#define CraftManager_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
        void setModel(const glm::mat4 & mat4) {
            glUniformMatrix4fv(glGetUniformLocation(shaderID, modelMat4Name.c_str()), 1,
                                GL_FALSE, glm::value_ptr(mat4));
        }
    public:
        CraftManager(GLfloat sizePerCube,
                    unsigned int numPerEdge,
                    unsigned int shaderID,
                    string mat4Name,
                    const vector<GLuint>& attriSize):modelMat4Name(mat4Name), shaderID(shaderID) {
            /**
             * 分配缓冲空间
             */
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            totalLengthPerVertex = COOR_DIMENSION + std::accumulate(attriSize.begin(), attriSize.end(), 0);
            
            /**
             * 初始化vertex vector，每个小立方体需要36个点
             */
            vertex = vector<GLfloat>(totalLengthPerVertex * TRIANGLE_PER_CUBE);
            // 初始化赋值，只考虑每个点的位置
            for (unsigned int i = 0; i < TRIANGLE_PER_CUBE; ++i) {
                unsigned int vertexInfoStartIndex = i * totalLengthPerVertex;
                unsigned int cubeVertexStartIndex = i * COOR_DIMENSION;
                for (int j = 0; j < COOR_DIMENSION; ++j) {
                    vertex[vertexInfoStartIndex + j] = cubeVertex[cubeVertexStartIndex + j] * sizePerCube;
                }
            }
            GLfloat* bufferTemp = new GLfloat[vertex.size()];
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex.front(), GL_STATIC_DRAW);
            // 三维的位置信息，为必选项
            glVertexAttribPointer(0, COOR_DIMENSION, GL_FLOAT, GL_FALSE, totalLengthPerVertex * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // 其他的属性，optional
            GLuint offset = COOR_DIMENSION;
            for (GLuint i = 1; i <= attriSize.size(); ++i) {
                glVertexAttribPointer(i, attriSize[i - 1], GL_FLOAT, GL_FALSE,
                                    totalLengthPerVertex * sizeof(GLfloat),
                                    (void*)(offset * sizeof(GLfloat)));
                glEnableVertexAttribArray(i);
                offset += attriSize[i - 1];
            }

           
            /**
             * 根据numPerEdge 初始化多个小立方体的位置
             */
            cubesPosition = vector<glm::vec3>();
            // 计算遍历的起始点
            GLfloat coorMin = -1 * sizePerCube * numPerEdge / 2 + sizePerCube / 2;
            glm::vec3 startPoint = glm::vec3(coorMin, coorMin, coorMin);
            for (unsigned int x = 0; x < numPerEdge; ++x) {
                for (unsigned int y = 0; y < numPerEdge; ++y) {
                    for (unsigned int z = 0; z < numPerEdge; ++z) {
                        cubesPosition.push_back(glm::vec3(x * sizePerCube, y * sizePerCube, z * sizePerCube) + startPoint);
                    }
                }
            }

            glBindVertexArray(0);
        }
        ~CraftManager() {
            /**
             * 销毁buffer
             */
            glDeleteVertexArrays(1, &VAO);
            glDeleteVertexArrays(1, &VBO);
        }

        void draw() {
            glBindVertexArray(VAO);
            for (unsigned int i = 0; i < cubesPosition.size(); ++i) {
                glm::mat4 model;
                model = glm::translate(model, cubesPosition[i]);
                setModel(model);
                glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_PER_CUBE);
            }
        }

        void setAttriArray(unsigned int offset, unsigned int size, const vector<GLfloat> & data) {
            offset += COOR_DIMENSION;
            for (unsigned int i = 0; i < TRIANGLE_PER_CUBE; ++i) {
                unsigned int startIndex = i * totalLengthPerVertex;
                unsigned int dataIndex = i * size;
                for (unsigned int j = 0; j < size; ++j) {
                    vertex[startIndex + j + offset] = data[dataIndex + j];
                }
            }
            // update
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(GLfloat), &vertex.front(), GL_STATIC_DRAW);
        }
};

#endif /* CraftManager_hpp */
