#ifndef __STATE_CONTROLLER_HEADER__
#define __STATE_CONTROLLER_HEADER__ 1

#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <math.h>

#include "./objects/GameObject.h"

#include "./objects/Grass.h"
#include "./objects/Dirt.h"
#include "./objects/Stone.h"
#include "./objects/Lamp.h"

#include "ResourcesManager.h"
#include "Camera.h"
#include "Shader.h"
#include "common.h"

struct tStateGameObject {
	GameObject* gameObject;
	bool shouldRender;
	bool operator==(const tStateGameObject& rhs) const {
		return gameObject == rhs.gameObject;
	}
};

typedef struct tStateGameObject tStateGameObject;

const int GROUND_INDEX = -5;

class StateController {
private:
	std::vector<tStateGameObject> objectsVector;
    std::list<tStateGameObject> objectsToRenderList;
	std::map<unsigned long, int> objectsMapByPosition;
    GLFWwindow* window;
    Camera* camera;
    Shader* shader;

	tLight light;
	int playerIndex;
	int objectiveIndex;

	ResourcesManager* resourcesManager;
	tObject groundObject;
	std::vector<GameObject*> groundGameObjectVector;
	int currentGOIndex;
	int groundObjectsCombined;
public:
    StateController (GLFWwindow* window, Camera* camera, ResourcesManager* rm);

	void reset();
    int addObject(GameObject* object);
	std::vector<tStateGameObject> getObjects();
	bool hasGroundAtPosition(glm::vec3 position);
	GameObject* getPlayer();
    GameObject* getObjective();
	GameObject* getGameObject(int idx);

	void jointObjects(bool reset = false);
    void update(float deltaTime);
    void render(float deltaTime);
};

#endif
