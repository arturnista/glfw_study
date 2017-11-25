#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__ 1

#include "GameObject.h"
#include "../Camera.h"
#include "../ResourcesManager.h"
#include "../StateController.h"
#include "../vendor/json.h"

using tJson = nlohmann::json;

class Player : public GameObject {
private:
    Camera* camera;

    bool firstMouse;
	double lastMouseX;
	double lastMouseY;

	float pitch;
    float yaw;
	float pitchLimit;

    float headBobbing;
    int headBobbingDirection;

    float mouseSens;

    StateController* stateController;

    float velocity;
    bool isGrounded();
    void applyPhysics(float deltaTime);
    void moveTo(glm::vec3 position);

public:
    Player (Camera* camera, ResourcesManager* rm, StateController* stateController);

    void setPosition(vec3 position);
    void update(GLFWwindow* window, float deltaTime);
};

#endif
