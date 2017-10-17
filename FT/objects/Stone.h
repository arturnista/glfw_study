#ifndef __STONE_HEADER__
#define __STONE_HEADER__ 1

#include "GameObject.h"

class Stone : public GameObject {
private:

public:
    Stone (ResourcesManager* tm);
    Stone (ResourcesManager* tm, float size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
