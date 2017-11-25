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

class StateController {
private:
	std::vector<tStateGameObject> objectsVector;
    std::list<tStateGameObject> objectsToRenderList;
	std::map<unsigned long, int> objectsMapByPosition;
    GLFWwindow* window;
    Camera* camera;
    Shader* shader;

	tLight light;

	ResourcesManager* resourcesManager;
	tObject groundObject;
	GameObject* groundGameObject;
public:
    StateController (GLFWwindow* window, Camera* camera, ResourcesManager* rm);

    void addObject(GameObject* object);
	std::vector<tStateGameObject> getObjects();
    tStateGameObject getObjectByPosition(glm::vec3 position);

	void jointObjects(bool reset = false);
    void update(float deltaTime);
    void render(float deltaTime);
};

#endif
