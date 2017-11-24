#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__ 1

#include "GameObject.h"
#include "../Camera.h"
#include "../ResourcesManager.h"
#include "../vendor/json.h"

using tJson = nlohmann::json;

class Player : public GameObject {
private:
    Camera* camera;

    glm::vec3 position;

    bool firstMouse;
	double lastMouseX;
	double lastMouseY;

	float pitch;
    float yaw;
	float pitchLimit;

    float aeho;
    int aehoDirection;

    float mouseSens;

public:
    Player (Camera* camera, ResourcesManager* rm);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
