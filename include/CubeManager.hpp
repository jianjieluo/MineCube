/**
 * @brief Cube Manager, Manager cubes & GLBufferManager; Draw all cubes
 * 
 * @file CubeManager.hpp
 * @author Johnny Law, Bob Wu
 * @date 2018-06-23
 */
#ifndef CubeManager_hpp
#define CubeManager_hpp

#include <memory>
#include <string>
#include <vector>
#include <json.hpp> // use nlohmann::json
#include "Global.hpp"

using std::vector;
using std::shared_ptr;
using std::string;

// test json
using json = nlohmann::json;

class GLBufferManager;
class Cube;

/**
 * @brief CubeManager aim to manage a set of cubes
 * 
 */
class CubeManager {
   private:
    static const glm::vec3 yAxis;
    static const glm::vec3 xAxis;
    static const glm::vec3 infPos;
    unsigned int width;   // x
    unsigned int height;  // y
    unsigned int depth;   // z
    unsigned int totalCube;
    GLfloat sizePerCube;
    vector<glm::vec3> cubesOriginalPosition;
    vector<shared_ptr<Cube>> cubes;
    // glm::vec3 rotateAngle;
    GLfloat rotateAngleAroundX;
    GLfloat rotateAngleAroundY;
    GLfloat rotateSensivitiy = 1.0f;

    GLBufferManager& glBufferManager;

    /**
     * @brief Write ALL Cubes position to the GLBufferManager
     * 
     */
    void refreshModelMat4();

    /**
     * @brief Write a specific cube position to the GLBufferManager
     * 
     * @param id a index of vector CubeManager::cubes, used to find the specific cube.
     */
    void refreshModelMat4(const unsigned int& id);

    /**
     * @brief Calculate a specific cube's model mat4 from the position vector<glm::vec3> CubeManager::cubesOriginalPosition.
     * 
     * @param id a index of vector CubeManager::cubes, used to find the specific cube.
     * @return glm::mat4 the corresponded glm::mat4 of CubeManager::cubesOriginalPosition[id].
     */
    glm::mat4 calculateModelMat4(const unsigned int& id);

   public:
   /**
    * @brief Construct a new Cube Manager object
    * 
    * @param width 
    * @param height 
    * @param depth 
    * @param sizePerCube 
    */
    CubeManager(unsigned int width, unsigned int height, unsigned int depth,
                GLfloat sizePerCube);

    /**
     * @brief Construct a new Cube Manager object, restore from file
     * 
     * @param model_path 
     */
    CubeManager(string model_path) = delete;

    /**
     * @brief Destroy the Cube Manager object
     * 
     */
    ~CubeManager();

    /**
     * @brief store the model to file, haven't implemented.
     * 
     * @param model_path 
     */
    void dump(string model_path) {}

    /**
     * @brief Get the Cube object using the 3D coordinates (x, y, z).
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @return shared_ptr<Cube> The share_ptr of CubeManager::cubes[getId(x, y, z)].
     */
    shared_ptr<Cube> getCube(unsigned int x, unsigned int y, unsigned int z);

    /**
     * If there already have one cube,
     * over-write it.
     * The incoming cube's size can be any value, just
     * it may cover other cubes.
     */
    
    /**
     * @brief Set the Cube object. If there already have one cube, over-write it.
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @param color (glm::vec3) rgb color with alpha default is 1.0
     * @param shaderID No use currently.
     */
    void setCube(unsigned int x, unsigned int y, unsigned int z,
                 glm::vec3 color, const unsigned int shaderID);

    /**
     * @brief Set the Cube object. If there already have one cube, over-write it.
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @param color (glm::vec4) rgb color.
     * @param shaderID No use currently.
     */
    void setCube(unsigned int x, unsigned int y, unsigned int z,
                 glm::vec4 color, const unsigned int shaderID);

    /**
     * @brief Check whether the position (x, y, z) now holds a cube.
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @return true There is a cube in (x, y, z)
     * @return false There isn't a cubt in (x, y, z)
     */
    bool isThereACube(unsigned int x, unsigned int y, unsigned int z);

    /**
     * @brief Delete the cube in position (x, y, z), transfer it out of the screen.
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     */
    void deleteCube(unsigned int x, unsigned int y, unsigned int z);

    /**
     * @brief Draw ALL Cube instances. Call it in the main render loop.
     * 
     */
    void draw();

    /**
     * @brief Initialize the CubeManager's component such as vector cubes and other else.
     * 
     * @param shaderID NO USE CURRENTLY.
     * @param mat4Name NO USE CURRENTLY.
     */
    void defalut_init_all(const GLuint& shaderID, const string& mat4Name);

    /**
     * @brief Get the Id of a cube in CubeManager::cubes.
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @return unsigned int the id of a cube in CubeManager::cubes
     */
    unsigned int getId(unsigned int x, unsigned int y, unsigned int z);

    /**
     * @brief Get the Id object (DEPARTURE NOW !!!! NO USE CURRENTLY)
     * 
     * @param x 
     * @param y 
     * @param z 
     * @param w 
     * @return unsigned int 
     */
    unsigned int getId(unsigned int x, unsigned int y, unsigned int z,
                       unsigned int w);

    /**

     */
    
    /**
     * @brief Set the All Shader Id object
     * Change all shaderID of cubes managed by this CubeManager
     * This is not a cheap operation.
     * Warning: Calling this function will change ALL CUBE shader!
     * If you just modify part of cubes' shader, use Cube::setShaderId instead.
     * 
     * @param shaderID the shader program id
     */
    void setAllShaderId(const GLuint& shaderID);

    /**
     * @brief rotate cubes horizontally
     * 
     * @param offset 
     */
    void rotateHorizontal(const GLfloat& offset);

    /**
     * @brief rotate cubes vertically
     * 
     * @param offset 
     */
    void rotateVertical(const GLfloat& offset);

    /**
     * @brief Set the Rotate Sensivity object
     * 
     * @param rotateSensivitiy 
     */
    void setRotateSensivity(const GLfloat& rotateSensivitiy);

    /**
     * @brief Get the Model Mat4 of the cube in the position (x, y, z)
     * 
     * @param x X axis
     * @param y Y axis
     * @param z Z axis
     * @return glm::mat4 The mat4 of the cube in the position (x, y, z)
     */
    glm::mat4 getModelMat4(unsigned int x, unsigned int y, unsigned int z);
};

#endif /* CubeManager_hpp */