#include "LevelController.h"

LevelController::LevelController(GLFWwindow* window, ResourcesManager* rm, StateController* sc, Map* m, Camera* c) {
    this->resourcesManager = rm;
    this->window = window;
    this->stateController = sc;
    this->map = m;
    this->camera = c;

    this->configData = rm->getConfigData()["levels"];
    this->currentLevel = -1;
    this->state = 0;
}

void LevelController::restart() {

    if(this->state == 0) {
        this->stateController->getPlayer()->setPosition(vec3(0, 10, 0));
    } else if(this->state == 1) {
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

int LevelController::home() {
    stateController->reset();
    this->map->reset();

    this->state = 0;
    for (size_t x = 0; x < 10; x++) {
        for (size_t z = 0; z < 10; z++) {
            this->map->createAt(vec3(x, 0, z));
        }
    }

    Player* player = new Player(camera, resourcesManager, stateController);
    player->setPosition(vec3(0, 10, 0));
    stateController->addObject( player );

	Lamp* lampObject = new Lamp(resourcesManager);
    stateController->addObject( lampObject );

    Teletransport* tpPlayground = new Teletransport(glm::vec3(0, 0, 1.0f), resourcesManager);
    tpPlayground->setPosition(vec3(0, 1, 6));
    int fIndex = stateController->addObject( tpPlayground );

    Teletransport* gamePlayground = new Teletransport(glm::vec3(0, 1.0f, 0), resourcesManager);
    gamePlayground->setPosition(vec3(6, 1, 6));
    stateController->addObject( gamePlayground );

    Teletransport* loadPlayground = new Teletransport(glm::vec3(1.0f, 1.0f, 0), resourcesManager);
    loadPlayground->setPosition(vec3(6, 1, 0));
    stateController->addObject( loadPlayground );

    return fIndex;
}

bool LevelController::loadSaved() {
    stateController->reset();
    this->map->reset();
    tJson savedFile = readSaveFile();

    this->state = 1;
    this->currentLevel = savedFile["level"];

    this->loadLevel(vec3(
        savedFile["position"]["x"],
        savedFile["position"]["y"],
        savedFile["position"]["z"]
    ), true);
}

void LevelController::playground() {
    stateController->reset();
    this->map->reset();

    this->state = 2;
    this->map->create();

    Player* player = new Player(camera, resourcesManager, stateController);
    player->setPosition(vec3(5, 10, 5));
    stateController->addObject( player );

	Lamp* lampObject = new Lamp(resourcesManager);
    stateController->addObject( lampObject );
}

bool LevelController::nextLevel() {
    this->currentLevel++;
    this->loadLevel(vec3(0), false);
}

bool LevelController::loadLevel(glm::vec3 pos, bool usePlayer) {
    stateController->reset();
    this->map->reset();

    if(this->currentLevel >= this->configData.size()) {
        this->currentLevel = -1;
        return true;
    }
    this->state = 1;

    tJson levelConfig = this->configData[this->currentLevel];

    for (size_t i = 0; i < levelConfig["map"].size(); i++) {
        this->map->createAt(vec3(
            levelConfig["map"][i]["position"]["x"],
            levelConfig["map"][i]["position"]["y"],
            levelConfig["map"][i]["position"]["z"]
        ));
    }

    Player* player = new Player(camera, resourcesManager, stateController);
    if(!usePlayer) {
        player->setPosition(vec3(
            levelConfig["player"]["position"]["x"],
            levelConfig["player"]["position"]["y"],
            levelConfig["player"]["position"]["z"]
        ));
    } else {
        player->setPosition(pos);
    }

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
}

int LevelController::getCurrentLevel() {
    return this->currentLevel;
}
