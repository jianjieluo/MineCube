#include "CubeManager.hpp"
#include "Cube.hpp"
#include "crud.h"
#include <list>

#include <iostream>

using std::cout;
using std::endl;

// 输入当前选中面和选中块判断要添加方块的位置
glm::vec3 getAddPos(const glm::vec3& cubePos, const int plane) {
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
	return glm::vec3(new_x, new_y, new_z);
}

bool canAccess(int new_x, int new_y, int new_z, int numPerEdge) {
	return (new_x > -1 && new_y > -1 && new_z > -1 && new_x < numPerEdge && new_y < numPerEdge && new_z < numPerEdge);
}

void createCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, 
	glm::vec3 color, const unsigned int shaderID, int numPerEdge) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				cout << canAccess(i, j, k, numPerEdge) << endl;
				if (canAccess(i, j, k, numPerEdge) && !cubeManager.isThereACube(i, j, k)) {
					cubeManager.setCube(i, j, k, color, shaderID);
				}
			}
}

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

vector<CubeInfo> getCubesInfo(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	vector<CubeInfo> cubeInfos;

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				if (canAccess(i, j, k, numPerEdge)) {
					auto cube = cubeManager.getCube(i, j, k);
					cubeInfos.push_back(CubeInfo(cube->isDeleted(), cube->getColor()));
				}
			}

	return cubeInfos;
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

void selectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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
					cube->selected();
				}
			}
}

void unselectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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
					cube->unselected();
				}
			}
}

void undoAdd(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
		const std::vector<CubeInfo> constCubeInfos) {
	if (constCubeInfos.empty())
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
				if (canAccess(i, j, k, numPerEdge)) {
					if (constCubeInfos[index].isDelected) {
						cubeManager.deleteCube(i, j, k);
					}
					index++;
				}
			}
}

void undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, 
		const std::vector<CubeInfo> constCubeInfos) {
	if (constCubeInfos.empty())
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
				if (!constCubeInfos[index].isDelected) {
					cubeManager.setCube(i, j, k, constCubeInfos[index].color, 0);
				}
				index++;
			}
}

void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos, 
		const glm::vec3& endCubePos, const std::vector<CubeInfo> constCubeInfos) {
	if (constCubeInfos.empty())
		return;
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	int index = 0;

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++) {
				auto cube = cubeManager.getCube(i, j, k);
				if (!cube->isDeleted()) {
					glm::vec4 color = constCubeInfos[index].color;
					cube->editColor(color.x, color.y, color.z, 1);
				}
				index++;
			}
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

//void recoverCubeColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec4>& savedColorList) {
//	if (savedColorList.empty())
//		return;
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
//				auto cube = cubeManager.getCube(i, j, k);
//				if (!cube->isDeleted()) {
//					glm::vec3 color = savedColorList.front();
//					for (int plane = 0; plane < 6; plane++)
//						cube->editColor(color.x, color.y, color.z, plane);
//					savedColorList.pop_front();
//				}
//			}
//}
//
//void saveRecoverColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec4>& savedColorList) {
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
//				auto cube = cubeManager.getCube(i, j, k);
//				if (!cube->isDeleted()) {
//					savedColorList.push_back(cube->getColor());
//				}
//			}
//}

//vector<bool> getCubesIsDeleted(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
//	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//
//	vector<bool> saveCubesIsDeleted;
//
//	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
//		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
//			for (unsigned int k = z_low_bound; k <= z_high_bound; k++)
//				saveCubesIsDeleted.push_back(cubeManager.getCube(i, j, k)->isDeleted());
//	return saveCubesIsDeleted;
//}
//
//vector<glm::vec4> getCubesColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
//	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
//	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
//	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
//
//	vector<glm::vec4> saveCubesColor;
//
//	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
//		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
//			for (unsigned int k = z_low_bound; k <= z_high_bound; k++)
//				saveCubesColor.push_back(cubeManager.getCube(i, j, k)->getColor());
//	return saveCubesColor;
//}

