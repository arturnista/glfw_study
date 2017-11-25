#ifndef __ENEMY_HEADER__
#define __ENEMY_HEADER__ 1

#include "GameObject.h"
#include "../ResourcesManager.h"
#include "../StateController.h"
#include "../vendor/json.h"

using tJson = nlohmann::json;

class Enemy : public GameObject {
private:
    StateController* stateController;

    float velocity;
    bool isGrounded();
    void applyPhysics(float deltaTime);
    void moveTo(glm::vec3 position);

public:
    Enemy (ResourcesManager* rm, StateController* stateController);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
