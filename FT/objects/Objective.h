#ifndef __OBJECTIVE_HEADER__
#define __OBJECTIVE_HEADER__ 1

#include "GameObject.h"
#include "../StateController.h"

class Objective : public GameObject {
private:
    StateController* stateController;

public:
    Objective (ResourcesManager* tm, StateController* stateController);
    Objective (ResourcesManager* tm, StateController* stateController, float size);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
