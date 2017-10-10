#ifndef __CUBE_HEADER__
#define __CUBE_HEADER__ 1

#include "GameObject.h"

class Cube : public GameObject {
private:

public:
    Cube (TexturesManager* tm);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
