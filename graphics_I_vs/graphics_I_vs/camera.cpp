#include "camera.h"


glm::vec3 cameraPos;
glm::vec3 oldCameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
glm::mat4 rotateMatrix;
bool lock;
bool clipping;
float cameraSpeed;
float rotational_speed;

Camera::Camera() {
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	oldCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraSpeed = 0.05f;
	lock = true;
	clipping = true;
	rotational_speed = 0.5f;
}

void Camera::addRotationalSpeed() {
	rotational_speed += 0.1f;
}

void Camera::takeRotationalSpeed() {
	if (rotational_speed >= 0.0f) {
		rotational_speed -= 0.1f;
	}
}

void Camera::mouseUpdate(glm::vec2 mouseDelta) {
	glm::vec3 toRotateArund = glm::cross(cameraFront, cameraUp);

	rotateMatrix =
		glm::rotate(-mouseDelta.x * rotational_speed, cameraUp) *
		glm::rotate(-mouseDelta.y * rotational_speed, toRotateArund);

	cameraFront = glm::mat3(rotateMatrix) * cameraFront;
}

void Camera::changeLock() {
	if (lock) { lock = false; }
	else { lock = true; }
}

void Camera::changeClipping() {
	if (clipping) { clipping = false; }
	else { clipping = true; }
}

glm::vec3 Camera::getCameraPos() {
	return cameraPos;
}

float Camera::getRotationSpeed() {
	return rotational_speed;
}

void Camera::setCameraPos(glm::vec3 camera) {
	cameraPos = camera;
}

glm::vec3 Camera::getCameraFront() {
	return cameraFront;
}

void Camera::setCameraFront(glm::vec3 camera) {
	cameraFront = camera;
}

glm::vec3 Camera::getCameraUp() {
	return cameraUp;
}

void Camera::setCameraUp(glm::vec3 camera) {
	cameraUp = camera;
}

glm::vec3 Camera::getOldCameraPos() {
	return oldCameraPos;
}

void Camera::setOldCameraPos(glm::vec3 camera) {
	oldCameraPos = camera;
}

bool Camera::isClipping() {
	return clipping;
}

void Camera::setClipping(bool c) {
	clipping = c;
}

bool Camera::isLock() {
	return lock;
}

void Camera::setLock(bool l) {
	lock = l;
}

void Camera::setCameraSpeed(float speed) {
	cameraSpeed = speed;
}

float Camera::getCameraSpeed() {
	return cameraSpeed;
}

void Camera::moveRight() {
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::moveLeft() {
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::moveForward() {
	cameraPos += cameraSpeed * cameraFront;
}

void Camera::moveBackwards() {
	cameraPos -= cameraSpeed * cameraFront;
}

void Camera::cameraPostoOld() {
	cameraPos = oldCameraPos;
}
void Camera::cameraOldtoPos() {
	oldCameraPos = cameraPos;
}

void Camera::moveUp() {
	cameraPos += cameraSpeed * cameraUp;
}

void Camera::moveDown() {
	cameraPos -= cameraSpeed * cameraUp;
}

void Camera::addSpeed() {
	cameraSpeed += 0.05f;
}

void Camera::lesSpeed() {
	printf("%f", cameraSpeed);
	if (cameraSpeed >= 0.0f) {
		printf("%f", cameraSpeed);
		cameraSpeed -= 0.05f;
	}
}