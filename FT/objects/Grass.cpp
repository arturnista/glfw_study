#include "Grass.h"

Grass::Grass(ResourcesManager* tm) : GameObject(tm, GO_TYPE_GROUND, "grass_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("objects");
}

Grass::Grass(ResourcesManager* tm, float size) : GameObject(tm, GO_TYPE_GROUND, "grass_normal", vec3(size), vec3(1.0f)) {
    this->setTextureName("objects");
}

void Grass::update(GLFWwindow* window, float deltaTime) {
}
