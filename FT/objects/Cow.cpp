#include "Cow.h"

Cow::Cow(ResourcesManager* tm) : GameObject(tm, GO_TYPE_ENEMY, "cow", vec3(1.0f), vec3(1.0f)) {

}

Cow::Cow(ResourcesManager* tm, float size) : GameObject(tm, GO_TYPE_ENEMY, "cow", vec3(size), vec3(1.0f)) {

}

void Cow::update(GLFWwindow* window, float deltaTime) {

}
