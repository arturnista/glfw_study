#ifndef __TELETRANSPORT_HEADER__
#define __TELETRANSPORT_HEADER__ 1

#include "GameObject.h"
#include "../StateController.h"

class Teletransport : public GameObject {
private:

public:
    Teletransport (glm::vec3 color, ResourcesManager* tm);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
