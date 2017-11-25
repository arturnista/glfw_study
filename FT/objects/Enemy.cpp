#include "Enemy.h"

Enemy::Enemy(ResourcesManager* rm, StateController* stateController) : GameObject(rm, GO_TYPE_ENEMY, "cube", vec3(1.0f), vec3(1.0f)) {
    // this->setTextureName("grass");
    this->stateController = stateController;

    this->velocity = 0.0f;

    tJson confiData = rm->getConfigData();

    this->position = vec3(
        confiData["enemy"]["position"]["x"],
        confiData["enemy"]["position"]["y"],
        confiData["enemy"]["position"]["z"]
    );
}

bool Enemy::isGrounded() {
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

void Enemy::applyPhysics(float deltaTime) {
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

void Enemy::moveTo(glm::vec3 position) {
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


void Enemy::update(GLFWwindow* window, float deltaTime) {
    float enemySpeed = 10.0f * deltaTime;
    glm::vec3 nextPosition = glm::vec3(this->position);

    glm::vec3 enemyFrontMovement = glm::vec3(1, 0, 0);

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    //     nextPosition += enemySpeed * enemyFrontMovement;
    // } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    //     nextPosition -= enemySpeed * enemyFrontMovement;
    // }
    //
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    //     nextPosition -= normalize(cross(enemyFrontMovement, glm::vec3(0, 1, 0))) * enemySpeed;
    // } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    //     nextPosition += normalize(cross(enemyFrontMovement, glm::vec3(0, 1, 0))) * enemySpeed;
    // }

    this->moveTo(nextPosition);
    this->applyPhysics(deltaTime);

    // std::cout << glm::to_string(this->position) << '\n';
}
