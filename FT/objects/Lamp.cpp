#include "Lamp.h"

Lamp::Lamp(ResourcesManager* rm) : GameObject(rm, GO_TYPE_LAMP, "cube", .3, vec3(1.0f)) {
    tJson confiData = rm->getConfigData();

    this->position = vec3(
        confiData["light"]["position"]["x"],
        confiData["light"]["position"]["y"],
        confiData["light"]["position"]["z"]
    );
}
