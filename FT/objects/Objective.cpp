#include "Objective.h"

Objective::Objective(ResourcesManager* tm, StateController* stateController) : GameObject(tm, GO_TYPE_OBJECTIVE, "objective_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("objects");
    this->stateController = stateController;
}

Objective::Objective(ResourcesManager* tm, StateController* stateController, float size) : GameObject(tm, GO_TYPE_OBJECTIVE, "objective_normal", vec3(size), vec3(1.0f)) {
    this->setTextureName("objects");
    this->stateController = stateController;
}

void Objective::update(GLFWwindow* window, float deltaTime) {
    this->rotateY(1.0f * deltaTime);
}
