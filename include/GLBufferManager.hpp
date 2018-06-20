#ifndef GLBufferManager_hpp
#define GLBufferManager_hpp

#include "Global.hpp"
#include "Cube.hpp"
#include "CubeManager.hpp"
#include <vector>
#include <numeric>
#include <memory>
using std::shared_ptr;
using std::vector;

/**
 * This Class is used to Manager the data of VAO/VBO
 * Singleton
 */
class GLBufferManager {
    friend class CubeManager;
    friend class Cube;

    public:
        static GLBufferManager& getInstance() {
            static GLBufferManager instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }

    private:
        // singleton
        GLBufferManager();

        // private data
        GLuint VAO;
        GLuint cubeVBO;
        GLuint colorInstanceVBO;
        GLuint modelInstanceVBO;

        vector<GLfloat> aCubeData;
        vector<glm::vec3> colorVecs;
        vector<glm::mat4> modelMatrices;
        

        void bufferData();
        void updateCubeBufferData();
        void updateColorBufferData();
        void updateModelBufferData();
        


    public:
        // Singleton
        GLBufferManager(GLBufferManager const&) = delete;
        void operator=(GLBufferManager const&)  = delete;

        void init(const int _totalCubeNum);

        void setCubeData(const vector<GLfloat>& _aCubeData);
        void setAllColor(const vector<glm::vec3>& _colorVecs);
        void setAllModel(const vector<glm::mat4>& _modelMatrices);
        
        void setColor(const int _id, const glm::vec3 _aColor);
        void setModel(const int _id, const glm::mat4 _aModel);

        void bind();
        void unbind();
};
#endif