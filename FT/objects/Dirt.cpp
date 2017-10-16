#include "Dirt.h"

Dirt::Dirt(ResourcesManager* tm) : GameObject(tm, GO_TYPE_DIRT, "dirt_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("grass");
}

Dirt::Dirt(ResourcesManager* tm, float size) : GameObject(tm, GO_TYPE_DIRT, "dirt_normal", vec3(size), vec3(1.0f)) {
    this->setTextureName("grass");
}

void Dirt::update(GLFWwindow* window, float deltaTime) {

}
