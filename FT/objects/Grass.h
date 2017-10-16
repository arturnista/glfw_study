#ifndef __GRASS_HEADER__
#define __GRASS_HEADER__ 1

#include "GameObject.h"

class Grass : public GameObject {
private:

public:
    Grass (ResourcesManager* tm);
    Grass (ResourcesManager* tm, float size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
