#ifndef __LEVEL_CONTROLLER_HEADER__
#define __LEVEL_CONTROLLER_HEADER__ 1

#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <math.h>

#include "ResourcesManager.h"
#include "StateController.h"
#include "Map.h"
#include "Camera.h"

#include "./objects/Enemy.h"
#include "./objects/Player.h"
#include "./objects/Objective.h"
#include "./objects/Teletransport.h"
#include "./objects/Cow.h"
#include "./vendor/json.h"
#include "./utils/delay.h"
#include "./utils/readFiles.h"

#include "common.h"

using tJson = nlohmann::json;

class LevelController {
private:
	ResourcesManager* resourcesManager;
	StateController* stateController;
	Map* map;
	GLFWwindow* window;
	Camera* camera;

	int state;

	tJson configData;
	int currentLevel;

	bool loadLevel(glm::vec3 pos, bool usePlayer);
public:
    LevelController (GLFWwindow* window, ResourcesManager* rm, StateController* sc, Map* m, Camera* c);

	int getCurrentLevel();

	int home();

	bool loadSaved();

	void playground();

	void restart();
	bool nextLevel();
};

#endif
