// crud.hpp
#include <vector>
#include <list>

using namespace std;

void createCube(CubeManager& cubeManager, const glm::vec3& cubePos, const int plane, 
	glm::vec3& color, const unsigned int shaderID, int numPerEdge);

void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, const vector<bool> saveCubesIsDeleted);

void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos);

void paintCube(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, const glm::vec3& color);

void recoverCubeColor(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, list<glm::vec4>& savedColorList);

void saveRecoverColor(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, list<glm::vec4>& savedColorList);

void undoAdd(CubeManager& cubeManager, const glm::vec3 constCubePos, const int constHoverPlane);

void undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos,
	const glm::vec3& endCubePos, const vector<bool> saveCubesIsDeleted, list<glm::vec4> savedColorList);

vector<bool> getCubesIsDeleted(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos);

void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, std::list<glm::vec4> savedColorList);
