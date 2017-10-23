#ifndef __MAP_HEADER__
#define __MAP_HEADER__ 1

#include <iostream>
#include <thread>

#include "StateController.h"
#include "ResourcesManager.h"

#include "./objects/Grass.h"
#include "./objects/Dirt.h"
#include "./objects/Stone.h"

class Map {
private:
    ResourcesManager* resourcesManager;
    StateController* stateController;

    bool finished;

    int sizeX;
    int sizeZ;
    int height;

    int sizeXCreated;
    int sizeZCreated;
    int heightCreated;

public:
    Map (ResourcesManager* resourcesManager, StateController* stateController);
    bool create(int amount = 0);
    bool createOne();
    void break(glm::vec3 position);
};

#endif
