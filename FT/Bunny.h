#ifndef __BUNNY_HEADER__
#define __BUNNY_HEADER__ 1

#include "GameObject.h"

class Bunny : public GameObject {
private:
    /* data */

public:
    Bunny (string filename, float size, vec3 color);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
