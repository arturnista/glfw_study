#ifndef __MAP_HEADER__
#define __MAP_HEADER__ 1

#include <iostream>

#include "StateController.h"
#include "ResourcesManager.h"

#include "Grass.h"
#include "Dirt.h"
#include "Stone.h"

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
    void create();
    bool createOne();
};

#endif
