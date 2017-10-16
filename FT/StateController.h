#ifndef __STATE_CONTROLLER_HEADER__
#define __STATE_CONTROLLER_HEADER__ 1

#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include "GameObject.h"
#include "Camera.h"
#include "Shader.h"
#include "common.h"

typedef struct {
	GameObject* gameObject;
	bool shouldRender;
} tStateGameObject;

class StateController {
private:
    std::vector<tStateGameObject> objectsVector;
	std::map<unsigned long, tStateGameObject> objectsVectorByPosition;
    GLFWwindow* window;
    Camera* camera;
    Shader* shader;

    bool shouldRender(glm::vec3 position);
public:
    StateController (GLFWwindow* window, Camera* camera);

    void addObject(GameObject* object);
    std::vector<tStateGameObject> getObjects();

	void prepareObjects();
    void update(float deltaTime);
    void render(float deltaTime);
};

#endif
