#include "Enemy.h"

Enemy::Enemy(ResourcesManager* rm, StateController* stateController) : GameObject(rm, GO_TYPE_ENEMY, "stone_normal", vec3(1.0f), vec3(2.0f)) {
    this->setTextureName("grass");
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
        GameObject* groundGo = this->stateController->getObjectByPosition(groundPos).gameObject;
        if(groundGo->getType() == GO_TYPE_GROUND) {
            return true;
        }
        return false;
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
    float enemySpeed = 0.5f * deltaTime;
    glm::vec3 nextPosition = glm::vec3(this->position);

    glm::vec3 enemyFrontMovement = glm::vec3(1, 0, 0);
    GameObject* player = this->stateController->getPlayer();
    glm::vec3 playerPos = player->getPosition();

    if(playerPos.x > nextPosition.x) {
        nextPosition.x += enemySpeed;
    } else if(playerPos.x < nextPosition.x) {
        nextPosition.x -= enemySpeed;
    }

    if(playerPos.z > nextPosition.z) {
        nextPosition.z += enemySpeed;
    } else if(playerPos.z < nextPosition.z) {
        nextPosition.z -= enemySpeed;
    }

    this->moveTo(nextPosition);
    this->applyPhysics(deltaTime);
}