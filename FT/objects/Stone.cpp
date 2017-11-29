#include "Stone.h"

Stone::Stone(ResourcesManager* tm) : GameObject(tm, GO_TYPE_GROUND, "stone_normal", vec3(1.0f), vec3(1.0f)) {
    
}

Stone::Stone(ResourcesManager* tm, float size) : GameObject(tm, GO_TYPE_GROUND, "stone_normal", vec3(size), vec3(1.0f)) {
    
}

void Stone::update(GLFWwindow* window, float deltaTime) {

}
