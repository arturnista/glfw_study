#ifndef __PLAYER_HEADER__
#define __PLAYER_HEADER__ 1

#include "GameObject.h"
#include "../Camera.h"

class Player : public GameObject {
private:
    Camera* camera;

    glm::vec3 position;

	double lastMouseX;
	double lastMouseY;

	float pitch;
	float yaw;

    float mouseSens;

public:
    Player (Camera* camera);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
