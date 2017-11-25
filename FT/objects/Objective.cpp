#include "Objective.h"

Objective::Objective(ResourcesManager* tm) : GameObject(tm, GO_TYPE_GROUND, "stone_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("grass");
}

Objective::Objective(ResourcesManager* tm, float size) : GameObject(tm, GO_TYPE_GROUND, "stone_normal", vec3(size), vec3(1.0f)) {
    this->setTextureName("grass");
}

void Objective::update(GLFWwindow* window, float deltaTime) {

}
