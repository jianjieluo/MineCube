#include "CubeManager.hpp"
#include "Cube.hpp"
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
				cubeManager.deleteCube(i, j, k);
			}
}

vector<shared_ptr<Cube>> getCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
	unsigned int x_low_bound = glm::min(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_low_bound = glm::min(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_low_bound = glm::min(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));
	unsigned int x_high_bound = glm::max(static_cast<int>(startCubePos.x), static_cast<int>(endCubePos.x));
	unsigned int y_high_bound = glm::max(static_cast<int>(startCubePos.y), static_cast<int>(endCubePos.y));
	unsigned int z_high_bound = glm::max(static_cast<int>(startCubePos.z), static_cast<int>(endCubePos.z));

	vector<shared_ptr<Cube>> saveEraseCubes;

	for (unsigned int i = x_low_bound; i <= x_high_bound; i++)
		for (unsigned int j = y_low_bound; j <= y_high_bound; j++)
			for (unsigned int k = z_low_bound; k <= z_high_bound; k++)
				saveEraseCubes.push_back(cubeManager.getCube(i, j, k));
	return saveEraseCubes;
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
					for (int plane = 0; plane < 6; plane++)
						cube->editColor(color.x, color.y, color.z, plane);
				}
			}
}

void recoverCubeColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec3>& savedColorList) {
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

void saveRecoverColor(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec3>& savedColorList) {
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
		const vector<shared_ptr<Cube>> saveEraseCubes, std::list<glm::vec3> savedColorList) {
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
                /*
                Now you can use 2 method to determine whether a cube is deleted.
                1. A cube can call `isDeleted()` to check whether itself is deleted.    `share_ptr<Cube> -> isDeleted()`
                2. Use  `CubeManager::isThereACube(x, y, z)` to check whether in the position (x, y, z) has a Cube now.
                */
				if (saveEraseCubes[index]->isDeleted()) {
					glm::vec3 color = savedColorList.front();
                    // now you don't need to use 6 plane.  Just `share_ptr<Cube>->editColor(r, g, b, alpha=1.0)` is ok
				    saveEraseCubes[index]->editColor(color.x, color.y, color.z);
					//cubeManager.setCube(i, j, k, saveEraseCubes[index]);
					savedColorList.pop_front();
					index++;
				}
			}
}

void undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, std::list<glm::vec3> savedColorList) {
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


