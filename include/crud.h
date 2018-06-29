// CRUD.h
#ifndef CRUD_H
#define CRUD_H

#include "CubeManager.hpp"
#include "BasicOperation.hpp"
#include "OperationManager.hpp"
#include "Cube.hpp"
#include "Shader.hpp"
#include "Gui.hpp"
#include <vector>
#include <list>
#include <iostream>

struct CubeInfo
{
	bool isDelected;
	glm::vec4 color;
	CubeInfo(bool _isDelected, glm::vec4 _color) : isDelected(_isDelected), color(_color) {}
};

class CRUD {
public:
	static void CRUD::run(const bool &hit, Gui& gui, glm::vec3& startCubePos, glm::vec3& farCubePos,
		const glm::vec3& hoverCubePosCurrent, bool& isHitBefore, int& hoverPlaneLast,
		int& hoverPlaneCurrent, CubeManager& cubeManager, glm::vec3& hoverCubePosLast,
		Shader& phongShader);

private:
	static glm::vec3 getAddPos(const glm::vec3& cubePos, const int plane);

	static void createCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
		glm::vec3 color, const unsigned int shaderID, int numPerEdge);

	static void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos, const vector<bool> saveCubesIsDeleted);

	static void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos);

	static void paintCube(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos, const glm::vec3& color);

	static void undoAdd(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
		const std::vector<CubeInfo> constCubeInfos);

	static void undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos, const std::vector<CubeInfo> constCubeInfos);

	static vector<CubeInfo> getCubesInfo(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos);

	static void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos,
		const glm::vec3& endCubePos, const std::vector<CubeInfo> constCubeInfos);

	static void selectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos);

	static void unselectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos);

	static bool canAccess(int new_x, int new_y, int new_z, int numPerEdge);


};

#endif CRUD_H