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

void LevelController::restart() {

    if(this->currentLevel < this->configData.size()) {
        tJson levelConfig = this->configData[this->currentLevel];
        this->stateController->getPlayer()->setPosition(vec3(
            levelConfig["player"]["position"]["x"],
            levelConfig["player"]["position"]["y"],
            levelConfig["player"]["position"]["z"]
        ));
    } else {
        this->stateController->getPlayer()->setPosition(vec3(5, 10, 5));
    }
}

bool LevelController::nextLevel() {
    this->currentLevel++;

    stateController->reset();
    this->map->reset();

    if(this->currentLevel < this->configData.size()) {
        tJson levelConfig = this->configData[this->currentLevel];

        for (size_t i = 0; i < levelConfig["map"].size(); i++) {
            this->map->createAt(vec3(
                levelConfig["map"][i]["position"]["x"],
                levelConfig["map"][i]["position"]["y"],
                levelConfig["map"][i]["position"]["z"]
            ));
        }

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

        Objective* objective = new Objective(resourcesManager, stateController);
        objective->setPosition(vec3(
            levelConfig["objective"]["position"]["x"],
            levelConfig["objective"]["position"]["y"],
            levelConfig["objective"]["position"]["z"]
        ));
        stateController->addObject( objective );

    	Lamp* lampObject = new Lamp(resourcesManager);
        stateController->addObject( lampObject );
        return false;
    } else {
        this->map->create();

        Player* player = new Player(camera, resourcesManager, stateController);
        player->setPosition(vec3(5, 10, 5));
        stateController->addObject( player );

    	Lamp* lampObject = new Lamp(resourcesManager);
        stateController->addObject( lampObject );
        return true;
    }
}
