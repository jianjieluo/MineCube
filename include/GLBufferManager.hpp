//
//  GLBufferManager.hpp
//
//  Created by 吴博文 on 2018/5/21.
//  Copyright © 2018年 吴博文. All rights reserved.
//

#ifndef GLBufferManager_hpp
#define GLBufferManager_hpp

#include "Global.hpp"
#include <vector>
#include <numeric>

using std::vector;
/**
 * This Class is used to Manager the data of VAO/VBO
 */
class GLBufferManager {
    private:
        GLuint VAO;
        GLuint VBO;
        GLuint totalLengthPerVertex;
        GLuint numRecord;
        vector<GLfloat> data;
        void bufferData();
    public:
        GLBufferManager() = delete;
        GLBufferManager(const vector<GLuint> & attriSize, GLuint numRecord);
        ~GLBufferManager();
        void setAttriArray(GLuint offset, GLuint size, const vector<GLfloat> & data);
        void bind();
        void unbind();
};
#endif