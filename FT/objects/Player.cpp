#include "Player.h"

Player::Player(Camera* camera, ResourcesManager* rm) : GameObject(rm, GO_TYPE_PLAYER) {
    this->camera = camera;

    this->firstMouse = true;
    this->lastMouseX = 0.0f;
    this->lastMouseY = 0.0f;
    this->pitch = 0;
    this->yaw = 0;
    this->pitchLimit = 70.0f;

    this->aehoDirection = 0;
    this->aeho = 0.0f;

    this->mouseSens = 0.5f;
    tJson confiData = rm->getConfigData();

    this->position = vec3(
        confiData["player"]["position"]["x"],
        confiData["player"]["position"]["y"],
        confiData["player"]["position"]["z"]
    );
    camera->setPosition(this->position);
}

void Player::update(GLFWwindow* window, float deltaTime) {
    // Get the cursor position
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if(this->firstMouse) {
        this->lastMouseY = mouseY;
        this->lastMouseX = mouseX;
        this->firstMouse = false;
    }

    // Compute the mouse position
    float mouseOffsetX = (mouseX - this->lastMouseX) * mouseSens;
    float mouseOffsetY = (this->lastMouseY - mouseY) * mouseSens;
    this->lastMouseY = mouseY;
    this->lastMouseX = mouseX;

    this->pitch += mouseOffsetY;
    this->yaw += mouseOffsetX;
    if(this->pitch > this->pitchLimit) this->pitch =  this->pitchLimit;
    if(this->pitch < -this->pitchLimit) this->pitch = -this->pitchLimit;

    vec3 playerFrontMovement = normalize( vec3(cos(radians(this->yaw)), 0, sin(radians(this->yaw))) );

    float playerSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->position += playerSpeed * playerFrontMovement;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->position -= playerSpeed * playerFrontMovement;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->position -= normalize(cross(playerFrontMovement, camera->getUp())) * playerSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->position += normalize(cross(playerFrontMovement, camera->getUp())) * playerSpeed;
    }

    vec3 lookingDirection = vec3(
        cos(radians(this->pitch)) * cos(radians(this->yaw)),
        sin(radians(this->pitch)),
        cos(radians(this->pitch)) * sin(radians(this->yaw))
    );
    camera->setFront( normalize(lookingDirection) );

    vec3 cameraPos = vec3( this->position );

    if( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ) {
        if(this->aehoDirection == 1) {
            this->aeho += 1.0f * deltaTime;
            if(this->aeho > 0.1f) this->aehoDirection = 0;
        } else {
            this->aeho -= 1.0f * deltaTime;
            if(this->aeho < -0.1f) this->aehoDirection = 1;
        }
    }
    cameraPos.y += 1.0f + this->aeho;
    camera->setPosition( cameraPos );
}
