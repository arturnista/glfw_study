#include "Player.h"

Player::Player(Camera* camera) : GameObject(NULL, GO_TYPE_PLAYER) {
    this->camera = camera;

    this->lastMouseX = 0.0f;
    this->lastMouseY = 0.0f;
    this->pitch = 0;
    this->yaw = 0;

    this->mouseSens = 3.0f;

    this->position = vec3(0.0f, 5.0f, 0.0f);
    camera->setPosition(this->position);
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

    vec3 playerFront = normalize(lookingDirection);

    float playerSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->position += playerSpeed * playerFront;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->position -= playerSpeed * playerFront;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->position -= normalize(cross(playerFront, camera->getUp())) * playerSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->position += normalize(cross(playerFront, camera->getUp())) * playerSpeed;
    }

    camera->setFront( playerFront );
    camera->setPosition(this->position);
}
