#include "Map.h"

Map::Map (ResourcesManager* resourcesManager, StateController* stateController) {
    this->resourcesManager = resourcesManager;
    this->stateController = stateController;

    this->sizeX = 40;
    this->sizeZ = 40;
    this->height = 10;

    this->sizeXCreated = 0;
    this->sizeZCreated = 0;
    this->heightCreated = 0;

    this->finished = false;
}

void Map::create() {
    while(this->createOne()) {
        
    }
}

bool Map::createOne() {
    if(this->finished) return false;

    if(this->heightCreated == 0) {
        Grass* grassObject = new Grass(this->resourcesManager);
        grassObject->setPosition(vec3(this->sizeXCreated, this->heightCreated * -1.0f, this->sizeZCreated));
        this->stateController->addObject( grassObject );
    } else if(this->heightCreated < 3) {
        Dirt* dirtObject = new Dirt(this->resourcesManager);
        dirtObject->setPosition(vec3(this->sizeXCreated, this->heightCreated * -1.0f, this->sizeZCreated));
        this->stateController->addObject( dirtObject );
    } else {
        Stone* stoneObject = new Stone(this->resourcesManager);
        stoneObject->setPosition(vec3(this->sizeXCreated, this->heightCreated * -1.0f, this->sizeZCreated));
        this->stateController->addObject( stoneObject );
    }


    this->sizeXCreated++;
    if(this->sizeX <= this->sizeXCreated) {
        this->sizeXCreated = 0;
        this->sizeZCreated++;

        if(this->sizeZ <= this->sizeZCreated) {
            this->sizeZCreated = 0;
            this->heightCreated++;

            if(this->height <= this->heightCreated) {
                this->finished = true;
            }
        }
    }
    return true;
}
