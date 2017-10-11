#ifndef __GRASS_HEADER__
#define __GRASS_HEADER__ 1

#include "GameObject.h"

class Grass : public GameObject {
private:

public:
    Grass (ResourcesManager* tm);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
