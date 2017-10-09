#ifndef __STATE_CONTROLLER_HEADER__
#define __STATE_CONTROLLER_HEADER__ 1

#include <iostream>
#include <vector>

#include "GameObject.h"
#include "Camera.h"

class StateController {
private:
    std::vector<GameObject*> objectsVector;

public:
    StateController (GLFWwindow* window, Camera* camera);

    void addObject(GameObject* object);
    std::vector<GameObject*> getObjects();
};

#endif
