#include "LevelController.h"

LevelController::LevelController(GLFWwindow* window, ResourcesManager* rm, StateController* sc, Map* m, Camera* c) {
    this->resourcesManager = rm;
    this->window = window;
    this->stateController = sc;
    this->map = m;
    this->camera = c;

    this->configData = rm->getConfigData()["levels"];
    this->currentLevel = -1;
}

void LevelController::nextLevel() {
    this->currentLevel++;

    stateController->reset();
    this->map->reset();

    this->map->create();

    tJson levelConfig = this->configData[this->currentLevel];

    Player* player = new Player(camera, resourcesManager, stateController);
    player->setPosition(vec3(
        levelConfig["player"]["position"]["x"],
        levelConfig["player"]["position"]["y"],
        levelConfig["player"]["position"]["z"]
    ));

    stateController->addObject( player );
    for (size_t i = 0; i < levelConfig["enemies"].size(); i++) {
        Enemy* enemy = new Enemy(resourcesManager, stateController);
        enemy->setPosition(vec3(
            levelConfig["enemies"][i]["position"]["x"],
            levelConfig["enemies"][i]["position"]["y"],
            levelConfig["enemies"][i]["position"]["z"]
        ));
        enemy->setSpeed(levelConfig["enemies"][i]["speed"]);
        stateController->addObject( enemy );
    }

	Lamp* lampObject = new Lamp(resourcesManager);
    stateController->addObject( lampObject );
}
