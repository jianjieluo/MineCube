#include "CubeManager.hpp"

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
) {

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		(static_cast<float>(mouseX) / static_cast<float>(screenWidth) - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		(static_cast<float>(mouseY) / static_cast<float>(screenHeight) - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 lRayEnd_NDC(
		(static_cast<float>(mouseX) / static_cast<float>(screenWidth) - 0.5f) * 2.0f,
		(static_cast<float>(mouseY) / static_cast<float>(screenHeight) - 0.5f) * 2.0f,
		0.0,
		1.0f
	);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}


bool TestRayOBBIntersection(
	glm::vec3 ray_origin,        // Ray origin, in world space
	glm::vec3 ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
	glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
	glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
) {

	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	glm::vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	glm::vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
											 // t1 and t2 now contain distances betwen ray origin and ray-plane intersections

											 // We want t1 to represent the nearest intersection, 
											 // so if it's not the case, invert t1 and t2
			if (t1>t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
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

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
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

			if (t1>t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
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
	const glm::vec3& hoverColor,
	const glm::vec3& objectColor,
	glm::vec3& lastHoverCubePos
) {
	glm::vec3 ray_origin;
	glm::vec3 ray_direction;
	ScreenPosToWorldRay(xpos, screenHeight - ypos, screenWidth, screenHeight, \
						view, projection, ray_origin, ray_direction);
	ray_direction = ray_direction * 20.0f;

	int cube_num = -1, hit_x = -1, hit_y = -1, hit_z = -1;
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

		if (this_cube && TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max,
			model_mat, intersection_distance) && intersection_distance < min_distance) {
			min_distance = intersection_distance;
			hit_x = x;
			hit_y = y;
			hit_z = z;
		}
	}

	// reset last hover cube
	auto last_hover_cube = cubeManager.getCube(static_cast<int>(lastHoverCubePos.x), static_cast<int>(lastHoverCubePos.y), static_cast<int>(lastHoverCubePos.z));
	if (last_hover_cube)
		for (int plane = 0; plane < 6; plane++)
			last_hover_cube->editColor(objectColor.x, objectColor.y, objectColor.z, plane);


	// hit
	if (hit_x != -1) {
		auto hover_cube = cubeManager.getCube(hit_x, hit_y, hit_z);
		for (int plane = 0; plane < 6; plane++)
			hover_cube->editColor(hoverColor.x, hoverColor.y, hoverColor.z, plane);
		lastHoverCubePos.x = static_cast<float>(hit_x);
		lastHoverCubePos.y = static_cast<float>(hit_y);
		lastHoverCubePos.z = static_cast<float>(hit_z);
		return true;
	}
	else {
		return false;
	}
}
