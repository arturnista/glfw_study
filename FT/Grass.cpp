#include "Grass.h"

Grass::Grass(ResourcesManager* tm) : GameObject(tm, "grass_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("grass");
}

void Grass::update(GLFWwindow* window, float deltaTime) {
}
