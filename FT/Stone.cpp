#include "Stone.h"

Stone::Stone(ResourcesManager* tm) : GameObject(tm, "cube_normal", vec3(1.0f), vec3(1.0f)) {
    this->setTextureName("stone");
}

void Stone::update(GLFWwindow* window, float deltaTime) {

}
