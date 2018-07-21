#include "CubeManager.hpp"
#include "Cube.hpp"

void ScreenPosToWorldRay(
	int mouseX, int mouseY,
	int screenWidth, int screenHeight,
	glm::mat4 ViewMatrix,
	glm::mat4 ProjectionMatrix,
	glm::vec3& out_origin,
	glm::vec3& out_direction
) {
	glm::vec4 lRayStart_NDC(
		(static_cast<float>(mouseX) / static_cast<float>(screenWidth) - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		(static_cast<float>(mouseY) / static_cast<float>(screenHeight) - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		(static_cast<float>(mouseX) / static_cast<float>(screenWidth) - 0.5f) * 2.0f,
		(static_cast<float>(mouseY) / static_cast<float>(screenHeight) - 0.5f) * 2.0f,
		0.0,
		1.0f
	);


	// from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}


bool TestRayOBBIntersection(
	glm::vec3 ray_origin,
	glm::vec3 ray_direction,
	glm::vec3 aabb_min,
	glm::vec3 aabb_max,
	glm::mat4 ModelMatrix,
	float& intersection_distance,
	int& plane_num
) {
	// test for each object
	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.x) / f;
			float t2 = (e + aabb_max.x) / f;
			// our plane
			int left = 2, right = 3;

			if (t1 > t2) {
				// swap t1 and t2
				float w = t1; t1 = t2; t2 = w;
				int t = left; left = right; right = t;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin) {
				tMin = t1;
				plane_num = left;
			}
			// NO intersection.
			if (tMax < tMin)
				return false;
		} else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;
			
			int up = 4, bottom = 5;

			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w;
				int t = up; up = bottom; bottom = t;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin) {
				tMin = t1;
				plane_num = up;
			}
			if (tMin > tMax)
				return false;

		} else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;

			int front = 0, back = 1;

			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				int t = front; front = back; back = t;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin) {
				tMin = t1;
				plane_num = front;
			}
			if (tMin > tMax)
				return false;

		} else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}
	
	intersection_distance = tMin;
	return true;

}

bool PickOneCube(
	int xpos, int ypos,
	int screenWidth, int screenHeight,
	const glm::mat4& view,
	const glm::mat4& projection,
	unsigned int numPerEdge,
	float sizePerCube,
	CubeManager cubeManager,
	glm::vec3& hoverCubePosCurrent,
	int& plane_num_current
) {
	glm::vec3 ray_origin;
	glm::vec3 ray_direction;
	ScreenPosToWorldRay(xpos, screenHeight - ypos, screenWidth, screenHeight, \
						view, projection, ray_origin, ray_direction);
	ray_direction = ray_direction * 20.0f;

	int cube_num = -1, hit_x = -1, hit_y = -1, hit_z = -1, pick_plane_num = -1;
	float min_distance = 100000.0f;
	// Test each Oriented Bounding Box (OBB).
	for (int index = 0; index < glm::pow(numPerEdge, 3); index++) {
		int t_index = index;
		int x = t_index / static_cast<int>(glm::pow(numPerEdge, 2));
		t_index -= x * static_cast<int>(glm::pow(numPerEdge, 2));
		int y = t_index / glm::pow(numPerEdge, 1);
		t_index -= y * static_cast<int>(glm::pow(numPerEdge, 1));
		int z = t_index;

		float intersection_distance; // Output of TestRayOBBIntersection()
		
		// Original vertices Coordinate
		glm::vec3 aabb_min = glm::vec3(-0.5f, -0.5f, -0.5f) * sizePerCube;
		glm::vec3 aabb_max = glm::vec3(0.5f, 0.5f, 0.5f) * sizePerCube;

		glm::mat4 model_mat = cubeManager.getModelMat4(x, y, z);
		auto this_cube = cubeManager.getCube(x, y, z);

		if (!this_cube->isDeleted() && TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max,
			model_mat, intersection_distance, plane_num_current) && intersection_distance < min_distance) {
			min_distance = intersection_distance;
			hit_x = x;
			hit_y = y;
			hit_z = z;
			pick_plane_num = plane_num_current;
		}
	}

	// hit
	if (hit_x != -1) {
		hoverCubePosCurrent.x = static_cast<float>(hit_x);
		hoverCubePosCurrent.y = static_cast<float>(hit_y);
		hoverCubePosCurrent.z = static_cast<float>(hit_z);
		plane_num_current = pick_plane_num;
		return true;
	}
	else {
		return false;
	}
}
