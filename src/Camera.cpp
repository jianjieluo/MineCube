#include "Camera.hpp"
#include <iostream>
// default value
const float YAW          = -90.0f;
const float PITCH        = 0.0f;
const float SPEED        = 2.5f;
const float SENSITIVITY  = 0.01f;
const float ZOOM         = 45.0f;

Camera::Camera() {
    cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);;
    targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = YAW;
    pitch = PITCH;
    moveSpeed = SPEED;
    mouseSensitivity = SENSITIVITY;
	zoomFactor = ZOOM;
	isFirstMove = true;
    updateCamera();
}

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &wup) {
    cameraPosition = position;
    targetPosition = target;
    worldUp = wup;
    yaw = YAW;
    pitch = PITCH;
    moveSpeed = SPEED;
    mouseSensitivity = SENSITIVITY;
	zoomFactor = ZOOM;
	isFirstMove = true;
    updateCamera();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

glm::vec3 Camera::getCameraPosition() {
    return cameraPosition;
}

float Camera::getZoomFactor() {
	return zoomFactor;
}

void Camera::moveCamera(const MoveDirection &direction, float deltaTime) {
    float actualSpeed = moveSpeed * deltaTime;
    if (direction == FORWARD)
        cameraPosition += cameraFront * actualSpeed;
    else if (direction == BACKWARD)
        cameraPosition -= cameraFront * actualSpeed;
    else if (direction == LEFT)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
    else if (direction == RIGHT)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * actualSpeed;
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
}

void Camera::zoomInOrOut(const float offsetY) {
    if (zoomFactor >= 1.0f && zoomFactor <= 45.0f)
        zoomFactor -= offsetY;
    if (zoomFactor <= 1.0f)
            zoomFactor = 1.0f;
    if (zoomFactor >= 45.0f)
        zoomFactor = 45.0f;
}

void Camera::pause() {
    isFirstMove = true;
}
