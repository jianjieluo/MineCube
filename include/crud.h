// crud.hpp
#include <vector>
#include <list>

using namespace std;

struct CubeInfo
{
	bool isDelected;
	glm::vec4 color;
	CubeInfo(bool _isDelected, glm::vec4 _color) : isDelected(_isDelected), color(_color) {}
};

glm::vec3 getAddPos(const glm::vec3& cubePos, const int plane);

void createCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
	glm::vec3 color, const unsigned int shaderID, int numPerEdge);

void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, const vector<bool> saveCubesIsDeleted);

void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos);

void paintCube(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, const glm::vec3& color);

void undoAdd(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
	const std::vector<CubeInfo> constCubeInfos);

void undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos,
	const glm::vec3& endCubePos, const std::vector<CubeInfo> constCubeInfos);

vector<CubeInfo> getCubesInfo(CubeManager& cubeManager, const glm::vec3& startCubePos,
	const glm::vec3& endCubePos);

void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos, 
	const glm::vec3& endCubePos, const std::vector<CubeInfo> constCubeInfos);

void selectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos);

void unselectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos);

bool canAccess(int new_x, int new_y, int new_z, int numPerEdge);

//vector<bool> getCubesIsDeleted(CubeManager& cubeManager, const glm::vec3& startCubePos, 
//	const glm::vec3& endCubePos);
//
//vector<glm::vec4> getCubesColor(CubeManager& cubeManager, const glm::vec3& startCubePos,
//	const glm::vec3& endCubePos);

//void recoverCubeColor(CubeManager& cubeManager, const glm::vec3& startCubePos, 
//	const glm::vec3& endCubePos, list<glm::vec4>& savedColorList);
//
//void saveRecoverColor(CubeManager& cubeManager, const glm::vec3& startCubePos, 
//	const glm::vec3& endCubePos, list<glm::vec4>& savedColorList);
