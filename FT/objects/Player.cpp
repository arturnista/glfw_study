#include "Player.h"

Player::Player(Camera* camera, ResourcesManager* rm, StateController* stateController) : GameObject(rm, GO_TYPE_PLAYER) {
    this->camera = camera;

    this->firstMouse = true;
    this->lastMouseX = 0.0f;
    this->lastMouseY = 0.0f;
    this->pitch = 0;
    this->yaw = 0;
    this->pitchLimit = 70.0f;

    this->headBobbingDirection = 0;
    this->headBobbing = 0.0f;

    this->velocity = 0.0f;

    this->mouseSens = 0.5f;
    tJson confiData = rm->getConfigData();

    this->position = vec3(
        confiData["player"]["position"]["x"],
        confiData["player"]["position"]["y"],
        confiData["player"]["position"]["z"]
    );
    camera->setPosition(this->position);

    this->stateController = stateController;
}

bool Player::isGrounded() {
    glm::vec3 groundPos = glm::vec3(this->position.x, this->position.y, this->position.z);
    groundPos.x = round(groundPos.x);
    groundPos.y -= 1.0f;
    groundPos.z = round(groundPos.z);

    float groundDiff = abs( groundPos.y - floor( groundPos.y ) );
    if(groundDiff > .1f) return false;

    groundPos.y = round( groundPos.y );

    try {
        this->stateController->getObjectByPosition(groundPos);
        return true;
    } catch(int e) {
        return false;
    }
}

void Player::applyPhysics(float deltaTime) {
    bool isGrounded = this->isGrounded();
    if(!isGrounded) {
        this->velocity -= 9.8f * deltaTime;
    }

    this->position.y += this->velocity * deltaTime;
    if(isGrounded) {
        this->velocity = 0.0f;
        this->position.y = floor( this->position.y );
    }
}

void Player::moveTo(glm::vec3 position) {
    glm::vec3 nextPos = glm::vec3( position );

    nextPos.x = round(nextPos.x);
    nextPos.y = round(nextPos.y);
    nextPos.z = round(nextPos.z);

    try {
        this->stateController->getObjectByPosition(nextPos);

        nextPos.x = round(nextPos.x);
        nextPos.y = round(nextPos.y);
        nextPos.z = round(this->position.z);
        try {
            this->stateController->getObjectByPosition(nextPos);

            nextPos.x = round(this->position.x);
            nextPos.y = round(nextPos.y);
            nextPos.z = round(nextPos.z);
            try {
                this->stateController->getObjectByPosition(nextPos);
            } catch(int e) {
                glm::vec3 xPosition = glm::vec3( position );
                xPosition.x = this->position.x;
                this->position = xPosition;
            }
        } catch(int e) {
            glm::vec3 zPosition = glm::vec3( position );
            zPosition.z = this->position.z;
            this->position = zPosition;
        }
    } catch(int e) {
        this->position = position;
    }
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
    vec3 nextPosition = vec3(this->position);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        nextPosition += playerSpeed * playerFrontMovement;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        nextPosition -= playerSpeed * playerFrontMovement;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        nextPosition -= normalize(cross(playerFrontMovement, camera->getUp())) * playerSpeed;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        nextPosition += normalize(cross(playerFrontMovement, camera->getUp())) * playerSpeed;
    }

    bool isGrounded = this->isGrounded();
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
        nextPosition.y += .2f;
        this->velocity = 5.0f;
    }
    this->moveTo(nextPosition);
    this->applyPhysics(deltaTime);

    vec3 lookingDirection = vec3(
        cos(radians(this->pitch)) * cos(radians(this->yaw)),
        sin(radians(this->pitch)),
        cos(radians(this->pitch)) * sin(radians(this->yaw))
    );
    camera->setFront( normalize(lookingDirection) );

    vec3 cameraPos = vec3( this->position );

    if( isGrounded && (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ) {
        if(this->headBobbingDirection == 1) {
            this->headBobbing += 1.0f * deltaTime;
            if(this->headBobbing > 0.1f) this->headBobbingDirection = 0;
        } else {
            this->headBobbing -= 1.0f * deltaTime;
            if(this->headBobbing < -0.1f) this->headBobbingDirection = 1;
        }
    } else {
        this->headBobbing = 0.0f;
    }
    cameraPos.y += 1.0f + this->headBobbing;
    camera->setPosition( cameraPos );
}
