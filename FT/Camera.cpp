#include "Camera.h"

Camera::Camera (GLFWwindow* window) {
    position = vec3(0.0f, 0.0f, -3.0f);
    front = vec3(0.0f, 0.0f, 0.0f);

    target = vec3(0.0f, 0.0f, 0.0f);
    direction = normalize(position - target);

    vec3 directionUp = vec3(0.0f, 1.0f, 0.0f);
    right = normalize(cross(directionUp, direction));
    up = cross(direction, right);

    this->window = window;
}

vec3 Camera::getUp() {
    return up;
}


vec3 Camera::getPosition() {
    return position;
}

void Camera::setPosition(vec3 position) {
    this->position = position;
}

vec3 Camera::getFront() {
    return front;
}

void Camera::setFront(vec3 front) {
    this->front = front;
}

mat4 Camera::getView() {
    return lookAt( position, position + front, up );
}

mat4 Camera::getProjection() {
	int screenWidth, screenHeight;
	glfwGetWindowSize(this->window, &screenWidth, &screenHeight);
	return perspective(radians(45.0f), float(screenWidth / screenHeight), 0.1f, 500.0f);
}
