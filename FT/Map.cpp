#include "Map.h"

Map::Map (ResourcesManager* resourcesManager, StateController* stateController) {
    this->resourcesManager = resourcesManager;
    this->stateController = stateController;

    this->sizeX = 60;
    this->sizeZ = 60;
    this->height = 1;

    this->sizeXCreated = 0;
    this->sizeZCreated = 0;
    this->heightCreated = 0;

    this->finished = false;
}

void Map::reset() {
    this->sizeXCreated = 0;
    this->sizeZCreated = 0;
    this->heightCreated = 0;

    this->finished = false;
}

bool Map::create(int amount) {
    float totalToCreate = this->sizeX * this->sizeZ;
    int created = 0;
    float lastPerc = 0.0f;

    if(amount > 0) {
        for (int i = 0; i < amount; i++) {
            bool ret = this->createOne();
            if(!ret) return false;
        }
        return true;
    } else {
        while (this->createOne()) {
            created++;
            int perc = floor((created / totalToCreate) * 100);
            if(perc % 5 == 0 && perc != lastPerc) {
                lastPerc = perc;
                std::cout << "Mapa " << perc << "%%" << '\n';
            }
        }
        return false;
    }
}

void Map::createAt(glm::vec3 position) {
    Stone* stoneObject = new Stone(this->resourcesManager);
    stoneObject->setPosition(position);
    this->stateController->addObject( stoneObject );
}

bool Map::createOne() {
    if(this->finished) return false;

    if(this->heightCreated == 0) {
        Grass* grassObject = new Grass(this->resourcesManager);
        grassObject->setPosition(vec3(this->sizeXCreated, this->heightCreated * -1.0f, this->sizeZCreated));
        this->stateController->addObject( grassObject );
    } else if(this->heightCreated < 2) {
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
            this->finished = true;
        }
    }

    if(this->sizeXCreated > 20 && this->sizeZCreated > 20 && this->sizeXCreated < 40 && this->sizeZCreated < 40) {
        if(this->sizeXCreated > 25 && this->sizeZCreated > 25 && this->sizeXCreated < 35 && this->sizeZCreated < 35) {
            this->heightCreated = 2;
        } else {
            this->heightCreated = 1;
        }
    } else {
        this->heightCreated = 0;
    }
    return true;
}
