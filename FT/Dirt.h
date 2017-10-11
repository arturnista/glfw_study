#ifndef __DIRT_HEADER__
#define __DIRT_HEADER__ 1

#include "GameObject.h"

class Dirt : public GameObject {
private:

public:
    Dirt (ResourcesManager* tm);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
