#include "Enemy.h"

Enemy::Enemy(ResourcesManager* rm, StateController* stateController) : GameObject(rm, GO_TYPE_ENEMY, "stone_normal", vec3(1.0f), vec3(2.0f)) {
    this->setTextureName("objects");
    this->stateController = stateController;

    this->velocity = 0.0f;
    this->position = vec3(0.0f);
}

void Enemy::setSpeed(float speed) {
    this->moveSpeed = speed;
}

bool Enemy::isGrounded() {
    glm::vec3 groundPos = glm::vec3(this->position.x, this->position.y, this->position.z);
    groundPos.x = round(groundPos.x);
    groundPos.y -= 1.0f;
    groundPos.z = round(groundPos.z);

    float groundDiff = abs( groundPos.y - floor( groundPos.y ) );
    if(groundDiff > .1f) return false;

    groundPos.y = round( groundPos.y );

    return this->stateController->hasGroundAtPosition(groundPos);
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

    bool hasGround = false;
    hasGround = this->stateController->hasGroundAtPosition(nextPos);
    if(!hasGround) {
        this->position = position;
    }

    nextPos.x = round(nextPos.x);
    nextPos.y = round(nextPos.y);
    nextPos.z = round(this->position.z);

    hasGround = this->stateController->hasGroundAtPosition(nextPos);
    if(!hasGround) {
        glm::vec3 zPosition = glm::vec3( position );
        zPosition.z = this->position.z;
        this->position = zPosition;
    }

    nextPos.x = round(this->position.x);
    nextPos.y = round(nextPos.y);
    nextPos.z = round(nextPos.z);

    hasGround = this->stateController->hasGroundAtPosition(nextPos);
    if(!hasGround) {
        glm::vec3 xPosition = glm::vec3( position );
        xPosition.x = this->position.x;
        this->position = xPosition;
    }
}


void Enemy::update(GLFWwindow* window, float deltaTime) {
    float enemySpeed = this->moveSpeed * deltaTime;
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

    float playerDistance = sqrt(
        pow(playerPos.x - this->position.x, 2) -
        sqrt(
            pow(playerPos.y - this->position.y, 2) -
            pow(playerPos.z - this->position.z, 2)
        )
    );

    if(playerDistance < 1) {
        std::cout << "PERDEU CUZAo" << '\n';
    }

    this->moveTo(nextPosition);
    this->applyPhysics(deltaTime);
}
