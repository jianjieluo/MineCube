#include "CubeManager.hpp"
#include "Cube.hpp"
#include "crud.h"
#include <list>

void createCube(CubeManager& cubeManager, const glm::vec3& cubePos, const int plane, glm::vec3& color, const unsigned int shaderID, int numPerEdge) {

	int new_x = static_cast<int>(cubePos.x), new_y = static_cast<int>(cubePos.y), new_z = static_cast<int>(cubePos.z);
	switch (plane) {
	case 0: new_z -= 1; break;
	case 1: new_z += 1; break;
	case 2: new_x -= 1; break;
	case 3: new_x += 1; break;
	case 4: new_y -= 1; break;
	case 5: new_y += 1; break;
	default: break;
	}
	if (new_x > -1 && new_y > -1 && new_z > -1 && new_x < numPerEdge && new_y < numPerEdge && new_z < numPerEdge)
		cubeManager.setCube(new_x, new_y, new_z, color, shaderID);
}

//void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, shared_ptr<vector<bool>>& saveCubesIsDeleted) {
//	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//
//	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
//		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
//			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
//				saveCubesIsDeleted->push_back(cubeManager.getCube(i, j, k)->isDeleted());
//				if (cubeManager.isThereACube(i, j, k)) {
//					cubeManager.deleteCube(i, j, k);
//				}
//			}
//}

void eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				if (cubeManager.isThereACube(i, j, k)) {
					cubeManager.deleteCube(i, j, k);
				}
			}
}

vector<bool> getCubesIsDeleted(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	vector<bool> saveCubesIsDeleted;

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++)
				saveCubesIsDeleted.push_back(cubeManager.getCube(i, j, k)->isDeleted());
	return saveCubesIsDeleted;
}

void paintCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, const glm::vec3& color) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				auto cube = cubeManager.getCube(i, j, k);
				if (cube) {
					cube->editColor(color.x, color.y, color.z, 1);
				}
			}
}

void recoverCubeColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec4>& savedColorList) {
	if (savedColorList.empty())
		return;
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				auto cube = cubeManager.getCube(i, j, k);
				if (!cube->isDeleted()) {
					glm::vec3 color = savedColorList.front();
					for (int plane = 0; plane < 6; plane++)
						cube->editColor(color.x, color.y, color.z, plane);
					savedColorList.pop_front();
				}
			}
}

void saveRecoverColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec4>& savedColorList) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				auto cube = cubeManager.getCube(i, j, k);
				if (!cube->isDeleted()) {
					savedColorList.push_back(cube->getColor());
				}
			}
}

void undoAdd(CubeManager& cubeManager, const glm::vec3 constCubePos, const int constHoverPlane) {
	glm::vec3 tempCubePos = constCubePos;
	switch (constHoverPlane) {
		case 0: tempCubePos.z -= 1; break;
		case 1: tempCubePos.z += 1; break;
		case 2: tempCubePos.x -= 1; break;
		case 3: tempCubePos.x += 1; break;
		case 4: tempCubePos.y -= 1; break;
		case 5: tempCubePos.y += 1; break;
		default: break;
	}
	eraseCube(cubeManager, tempCubePos, tempCubePos);
}

void undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, 
	const vector<bool> saveCubesIsDeleted, std::list<glm::vec4> savedColorList) {
	if (savedColorList.empty())
		return;
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int index = 0;

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				if (!saveCubesIsDeleted[index]) {
					glm::vec4 color = savedColorList.front();
					//	shaderID no use
					cubeManager.setCube(i, j, k, color, 0);
					savedColorList.pop_front();
					index++;
				}
			}
}

void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec4> savedColorList) {
	if (savedColorList.empty())
		return;
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				auto cube = cubeManager.getCube(i, j, k);
				if (!cube->isDeleted()) {
					glm::vec4 color = savedColorList.front();
					cube->editColor(color.x, color.y, color.z, 1);
					savedColorList.pop_front();
				}
			}
}


