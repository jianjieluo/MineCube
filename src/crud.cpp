#include "CRUD.h"
#include <list>

using std::cout;
using std::endl;

void CRUD::run(const bool &hit, Gui& gui, glm::vec3& startCubePos, glm::vec3& farCubePos,
		const glm::vec3& hoverCubePosCurrent, bool& isHitBefore, int& hoverPlaneLast,
		int& hoverPlaneCurrent, CubeManager& cubeManager, glm::vec3& hoverCubePosLast,
		Shader& phongShader) {
	//use to capture io
	ImGuiIO& io = ImGui::GetIO();

	if (hit && !gui.isSaveWindowShow()) {
		//若点击左键
		if (ImGui::IsMouseClicked(0)) {
			// 记录当前悬浮方块
			startCubePos = hoverCubePosCurrent;
			farCubePos = hoverCubePosCurrent;

			//用于拉出框体的操作
			isHitBefore = true;

			if (mode == CREATE_MODE) {
				hoverPlaneLast = hoverPlaneCurrent;
			}
		}
		//若按着左键
		if (io.MouseDown[0]) {
			if (farCubePos != hoverCubePosCurrent && isHitBefore) {
				unselectCubes(cubeManager, startCubePos, farCubePos);
				selectCubes(cubeManager, startCubePos, hoverCubePosCurrent);
				farCubePos = hoverCubePosCurrent;
			}
		}

		// set hovered plane
		auto hover_cube = cubeManager.getCube(hoverCubePosCurrent.x, hoverCubePosCurrent.y, hoverCubePosCurrent.z);
		if (!hover_cube->isDeleted()) {
			hoverCubePosLast = hoverCubePosCurrent;
			hover_cube->hitted();
		}
	}

	//若松开左键
	if (ImGui::IsMouseReleased(0)) {
		if (isHitBefore) {
			// 去除辅助色
			unselectCubes(cubeManager, startCubePos, farCubePos);

			// 保全当前全局变量
			glm::vec3 savedColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);
			glm::vec3 savedStartCubePos = startCubePos;
			glm::vec3 savedEndCubePos = hoverCubePosCurrent;

			if (mode == CREATE_MODE) {
				savedStartCubePos = getAddPos(savedStartCubePos, hoverPlaneLast);
				savedEndCubePos = getAddPos(savedEndCubePos, hoverPlaneCurrent);
			}

			const vector<CubeInfo> constCubesInfo = getCubesInfo(cubeManager, savedStartCubePos, savedEndCubePos);

			if (mode == ERASE_MODE) {
				auto eraseOP = shared_ptr<BasicOperation>(new BasicOperation(
					//	do 
					[&cubeManager, savedStartCubePos, savedEndCubePos]() {
					eraseCube(cubeManager, savedStartCubePos, savedEndCubePos);
				},
					//	undo
					[&cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo]() {
					undoErase(cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo);
				}
				));
				operationManager.executeOp(eraseOP, "Erase a cubes");
			}
			if (mode == PAINT_MODE) {
				auto paintOP = shared_ptr<BasicOperation>(new BasicOperation(
					//	do 
					[&cubeManager, savedColor, savedStartCubePos, savedEndCubePos]() {

					paintCube(cubeManager, savedStartCubePos, savedEndCubePos, savedColor);
				},
					//	undo
					[&cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo]() {
					undoPaint(cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo);
				}
				));
				operationManager.executeOp(paintOP, "Change cubes color");
			}
			if (mode == CREATE_MODE) {

				auto createOP = shared_ptr<BasicOperation>(new BasicOperation(
					//	do 
					[&cubeManager, savedColor, phongShader, savedStartCubePos, savedEndCubePos]() {
					createCube(cubeManager, savedStartCubePos, savedEndCubePos, savedColor, phongShader.ID, numPerEdge);
				},
					//	undo
					[&cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo]() {
					undoAdd(cubeManager, savedStartCubePos, savedEndCubePos, constCubesInfo);
				}
				));
				operationManager.executeOp(createOP, "Create a cube");
			}
		}
		isHitBefore = false;
	}

	if (gui.isFullNeed) {
		// 保全当前全局变量
		const vector<CubeInfo> constCubesInfo = getCubesInfo(cubeManager, glm::vec3(0, 0, 0), glm::vec3(numPerEdge - 1, numPerEdge - 1, numPerEdge - 1));
		glm::vec3 savedColor = glm::vec3(cubes_color[0], cubes_color[1], cubes_color[2]);

		auto createOP = shared_ptr<BasicOperation>(new BasicOperation(
			//	do 
			[&cubeManager, savedColor, phongShader]() {
			createCube(cubeManager, glm::vec3(0, 0, 0), glm::vec3(numPerEdge - 1, numPerEdge - 1, numPerEdge - 1), savedColor, phongShader.ID, numPerEdge);
		},
			//	undo
			[&cubeManager, constCubesInfo]() {
			undoAdd(cubeManager, glm::vec3(0, 0, 0), glm::vec3(numPerEdge - 1, numPerEdge - 1, numPerEdge - 1), constCubesInfo);
		}
		));
		operationManager.executeOp(createOP, "Use 'Full' tool");
		gui.isFullNeed = false;
	}
}

// 输入当前选中面和选中块判断要添加方块的位置
glm::vec3 CRUD::getAddPos(const glm::vec3& cubePos, const int plane) {
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

bool CRUD::canAccess(int new_x, int new_y, int new_z, int numPerEdge) {
	return (new_x > -1 && new_y > -1 && new_z > -1 && new_x < numPerEdge && new_y < numPerEdge && new_z < numPerEdge);
}

void CRUD::createCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
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
				if (canAccess(i, j, k, numPerEdge) && !cubeManager.isThereACube(i, j, k)) {
					cubeManager.setCube(i, j, k, color, shaderID);
				}
			}
}

void CRUD::eraseCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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

vector<CubeInfo> CRUD::getCubesInfo(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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


void CRUD::paintCube(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos, const glm::vec3& color) {
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

void CRUD::selectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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

void CRUD::unselectCubes(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos) {
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

void CRUD::undoAdd(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
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

void CRUD::undoErase(CubeManager& cubeManager, const glm::vec3& startCubePos, const glm::vec3& endCubePos,
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

void CRUD::undoPaint(CubeManager& cubeManager, const glm::vec3& startCubePos,
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

