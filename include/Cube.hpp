/**
 * @brief Cube class definition.
 * 
 * @file Cube.hpp
 * @author Johnny Law, Bob Wu
 * @date 2018-06-23
 */
#ifndef Cube_hpp
#define Cube_hpp

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <numeric>
#include <string>
#include <vector>
#include "CubeManager.hpp"
#include "Global.hpp"
using std::shared_ptr;
using std::vector;
using std::string;

class GLBufferManager;

/**
 * This is a base class for cube.
 * You can inherit it and create your own cube.
 */
class Cube {
    friend class CubeManager;

   private:
    static const unsigned int COOR_DIMENSION = 3;
    static const unsigned int NOR_VECTOR_DIMENSION = 3;
    static const unsigned int COLOR_DIMENSION = 3;
    static const unsigned int VERTEX_PER_CUBE = 36;
    static const unsigned int VERTEX_PER_PLANE = 6;

    // Static variable
    static const vector<GLfloat> cubeVertex;
    static const vector<GLfloat> cubeNormal;
    static const glm::vec4 initCubeColor;
    static const glm::vec4 hitColor;

    static const unsigned int POSITION_ATTRI_OFFSET = 0;
    static const unsigned int NOR_VECTOR_ATTRI_OFFSET = 3;
    static const unsigned int COLOR_ATTRI_OFFSET = 6;

    static const unsigned int LEFT_PLANE_BEGIN = 0;
    static const unsigned int RIGHT_PLANE_BEGIN = 0;
    static const unsigned int UP_PLANE_BEGIN = 0;
    static const unsigned int BOTTOM_PLANE_BEGIN = 0;
    static const unsigned int FRONT_PLANE_BEGIN = 0;
    static const unsigned int BACK_PLANE_BEGIN = 0;

    GLuint shaderID;
    GLBufferManager& glBufferManager;
    unsigned int id;
    glm::vec4 cubeColor;
    bool isInInf;

   public:
    Cube(unsigned int _id, GLfloat size, GLuint shaderID);

    Cube(GLfloat size, GLuint shaderID, const string& mat4Name = "model",
         vector<GLuint> attriSize = vector<GLuint>());

    Cube(const Cube&) = delete;
    ~Cube();

    /**
     * @brief edit this cube color.
     * 
     * @param r 
     * @param g 
     * @param b 
     * @param alpha Default is 1.0
     */
    virtual void editColor(GLfloat r, GLfloat g, GLfloat b,
                           GLfloat alpha = 1.0);

    /**
     * @brief Set the hitted cube's color as hitted color.
     * 
     */
    virtual void hitted();

    /**
     * @brief Resume the cube original color.
     * 
     */
    virtual void unhitted();
    
    /**
     * @brief Set the cube's color as selected color.(decrease alpha 0.5) 
     * 
     */
    virtual void selected();

    /**
     * @brief Resume the cube original color.
     * 
     */
    virtual void unselected();

    /**
     * @brief Write the Cube::cubeColor to the glBufferManager
     * 
     */
    virtual void cubeColor2Buffer();

    virtual glm::vec4 getColor();
    virtual glm::vec4 getColorOfPLane(unsigned int plane);

    /**
     * @brief Check whether the cube is deleted in the screen
     * 
     * @return true The cube does be deleted.
     * @return false The cube is not deleted.
     */
    bool isDeleted() const;

    virtual void setId(const unsigned int _id);
    virtual void setModelMat4(const glm::mat4&);
    virtual void setShaderId(const GLuint& shaderID);


    static const unsigned int LEFT = 0;
    static const unsigned int RIGHT = 1;
    static const unsigned int UP = 2;
    static const unsigned int BOTTOM = 3;
    static const unsigned int FRONT = 4;
    static const unsigned int BACK = 5;
};

#endif /* Cube_hpp */
