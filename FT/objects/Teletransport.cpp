#include "Teletransport.h"

Teletransport::Teletransport(glm::vec3 color, ResourcesManager* tm) : GameObject(tm, GO_TYPE_OBJECTIVE, "objective_normal", vec3(1.0f), color) {
    this->setTextureName("objects");
}

void Teletransport::update(GLFWwindow* window, float deltaTime) {
    this->rotateY(1.0f * deltaTime);
}
