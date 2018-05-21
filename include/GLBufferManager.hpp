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
#include <memory>
using std::shared_ptr;
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
        GLBufferManager(const vector<GLuint> & attriSize, const GLuint & numRecord);        
    public:
        GLBufferManager() = delete;
        /**
         * If you want this method,
         * feel free to contact me at bowen.wu@163.com
         */
        GLBufferManager& operator = (const GLBufferManager &) = delete;
        static shared_ptr<GLBufferManager> getNewInstance(const vector<GLuint> & attrisize, const GLuint & numRecord);        
        ~GLBufferManager();
        void setAttriArray(GLuint offset, GLuint size, const vector<GLfloat> & data);
        void bind();
        void unbind();
};
#endif