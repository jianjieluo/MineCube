#include "GLBufferManager.hpp"

GLBufferManager::GLBufferManager(const vector<GLuint> & attriSize, const GLuint & numRecord):numRecord(numRecord) {
    /**
	* 分配缓冲空间
	*/
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    totalLengthPerVertex = std::accumulate(attriSize.begin(), attriSize.end(), 0);    
    data = vector<GLfloat>(totalLengthPerVertex * numRecord);
    this->bufferData();
    for (GLuint i = 0, offset = 0; i < attriSize.size(); offset += attriSize[i], ++i) {
        glVertexAttribPointer(i, attriSize[i], GL_FLOAT, GL_FALSE,
                                totalLengthPerVertex * sizeof (GLfloat),
                                (void*)(offset * sizeof(GLfloat)));
        glEnableVertexAttribArray(i);
    }
}

GLBufferManager::~GLBufferManager() {
    /**
     * 销毁buffer
     */
    glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
}

void GLBufferManager::setAttriArray(GLuint offset, GLuint size, const vector<GLfloat> & in_data) {
    for (unsigned int i = 0; i < numRecord; ++i) {
        unsigned int startIndex = i * totalLengthPerVertex;
        unsigned dataIndex = i * size;
        for (unsigned int j = 0; j < size; ++j) {
            data[startIndex + j + offset] = data[dataIndex + j];
        }
    }
    this->bufferData();
}

void GLBufferManager::bind() {
    glBindVertexArray(VAO);
}

void GLBufferManager::unbind() {
    glBindVertexArray(0);
}

void GLBufferManager::bufferData() {
    // update
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data.front(), GL_STATIC_DRAW);
}

shared_ptr<GLBufferManager> GLBufferManager::getNewInstance(const vector<GLuint> & attrisize, const GLuint & numRecord) {
    return shared_ptr<GLBufferManager>(new GLBufferManager(attrisize, numRecord));
}