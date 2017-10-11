#ifndef __GRASS_HEADER__
#define __GRASS_HEADER__ 1

#include "GameObject.h"

class Grass : public GameObject {
private:

public:
    Grass (TexturesManager* tm, vec3 size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
