#include "Camera.hpp"
#include <iostream>
// default value
const float YAW          = -90.0f;
const float PITCH        = 0.0f;
const float SPEED_FIX    = 5.0f;
const float SPEED_MOVE   = 3.0f;
const float SENSITIVITY  = 0.005f;
const float ZOOM         = 45.0f;

Camera* Camera::instance = new Camera();

Camera* Camera::getInstance() {
	return instance;
}
Camera::Camera() {
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.5f);
    targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = YAW;
    pitch = PITCH;
    moveSpeed = SPEED_FIX;
    mouseSensitivity = SENSITIVITY;
	zoomFactor = ZOOM;
	isMoving = false;
	isFirstMove = true;
	isFpsMode = false;
	rotateX = 0.0f;
	rotateY = 0.0f;
    updateCamera();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPosition, targetPosition, cameraUp);
}

glm::vec3 Camera::getCameraPosition() const {
    return cameraPosition;
}

float Camera::getZoomFactor() const {
	return zoomFactor;
}

void Camera::moveCamera(const MoveDirection &direction, float deltaTime) {
	if (isFpsMode) {
		moveSpeed = SPEED_MOVE;
	}
	float actualSpeed = moveSpeed * deltaTime;
	if (isFpsMode) {
		if (direction == FORWARD)
			cameraPosition += cameraFront * actualSpeed;
		else if (direction == BACKWARD)
			cameraPosition -= cameraFront * actualSpeed;
		else if (direction == LEFT)
			cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
		else if (direction == RIGHT)
			cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
		targetPosition = cameraPosition + cameraFront;
	}
	else {
		if (direction == FORWARD)
			cameraPosition += cameraFront * actualSpeed;
		else if (direction == BACKWARD)
			cameraPosition -= cameraFront * actualSpeed;
		else if (direction == LEFT)
			rotateX -= 0.1f;
		else if (direction == RIGHT)
			rotateX += 0.1f;
	}
	if (isFpsMode) {
		moveSpeed = SPEED_FIX;
	}
}

void Camera::lookAround(const float currentX, const float currentY) {
    if (isFirstMove) {
        lastX = currentX;
        lastY = currentY;
        isFirstMove = false;
    }
    float offsetX = currentX - lastX;
    float offsetY = currentY - lastY;
    lastX = currentX;
    lastY = currentY;
    yaw += offsetX * mouseSensitivity;
    pitch -= offsetY * mouseSensitivity;

	// constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCamera();
}

void Camera::updateCamera() {
    cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraFront.y = sin(glm::radians(pitch));
    cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(cameraFront);

    cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));
    cameraUp = glm::cross(cameraFront, cameraRight);

	targetPosition = cameraPosition + cameraFront;
}

void Camera::zoomInOrOut(const float offsetY) {
    if (zoomFactor >= 1.0f && zoomFactor <= 45.0f)
        zoomFactor -= offsetY;
    if (zoomFactor <= 1.0f)
        zoomFactor = 1.0f;
    if (zoomFactor >= 45.0f)
        zoomFactor = 45.0f;
}

bool Camera::isRotateX() const {
	return glm::abs(getRotateX() - 0.0f) >= 0.001f;
}

float Camera::getRotateX() const {
	return rotateX;
}

void Camera::resetRotateX() {
	rotateX = 0.0f;
}

bool Camera::isRotateY() const {
	return glm::abs(getRotateY() - 0.0f) >= 0.001f;
}

float Camera::getRotateY() const {
	return rotateY;
}

void Camera::resetRotateY() {
	rotateY = 0.0f;
}

glm::vec2 Camera::updateXYoffset(float currentX, float currentY) {
	if (isFirstMove) {
		lastX = currentX;
		lastY = currentY;
		isFirstMove = false;
	}
	float offsetX = currentX - lastX;
	float offsetY = currentY - lastY;
	lastX = currentX;
	lastY = currentY;
	return glm::vec2(offsetX, offsetY);
}

void Camera::pause() {
    isFirstMove = true;
}
