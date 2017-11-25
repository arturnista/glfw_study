#ifndef __OBJECTIVE_HEADER__
#define __OBJECTIVE_HEADER__ 1

#include "GameObject.h"

class Objective : public GameObject {
private:

public:
    Objective (ResourcesManager* tm);
    Objective (ResourcesManager* tm, float size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
