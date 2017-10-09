#include "Player.h"

Player::Player(Camera* camera) : GameObject() {
    this->camera = camera;

    this->lastMouseX = 0.0f;
    this->lastMouseY = 0.0f;
    this->pitch = 0;
    this->yaw = 0;

    this->mouseSens = 2.0f;
}

void Player::update(GLFWwindow* window, float deltaTime) {
    // Get the cursor position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Compute the mouse position
    float mouseOffsetX = (mouseX - this->lastMouseX) * deltaTime * mouseSens;
    float mouseOffsetY = (this->lastMouseY - mouseY) * deltaTime * mouseSens;
    this->lastMouseY = mouseY;
    this->lastMouseX = mouseX;

    this->pitch += mouseOffsetY;
    this->yaw += mouseOffsetX;
    if(this->pitch > 89.0f) this->pitch =  89.0f;
    if(this->pitch < -89.0f) this->pitch = -89.0f;

    vec3 lookingDirection = vec3(
        cos(radians(this->pitch)) * cos(radians(this->yaw)),
        sin(radians(this->pitch)),
        cos(radians(this->pitch)) * sin(radians(this->yaw))
    );

    camera->setFront( normalize(lookingDirection) );
    vec3 cameraFront = camera->getFront();

    float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->setPosition( camera->getPosition() + (cameraSpeed * cameraFront) );
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->setPosition( camera->getPosition() - (cameraSpeed * cameraFront) );
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->setPosition( camera->getPosition() - (normalize(cross(cameraFront, camera->getUp())) * cameraSpeed) );
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->setPosition( camera->getPosition() + (normalize(cross(cameraFront, camera->getUp())) * cameraSpeed) );
    }
}
