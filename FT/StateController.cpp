#include "StateController.h"

StateController::StateController(GLFWwindow* window, Camera* camera) {
    objectsVector = {};
}

void StateController::addObject(GameObject* object) {
    objectsVector.push_back(object);
}

std::vector<GameObject*> StateController::getObjects() {
    return objectsVector;
}
