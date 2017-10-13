#include "StateController.h"

struct myclass {
    inline bool operator() (tStateGameObject struct1, tStateGameObject struct2) {
        return struct1.gameObject->getType() < struct2.gameObject->getType();
    }
} objectSort;


StateController::StateController(GLFWwindow* window, Camera* camera) {
    objectsVector = {};
    this->window = window;
    this->camera = camera;
    this->shader = new Shader("texture");
}

bool StateController::shouldRender(glm::vec3 position) {
    bool isRight = false;
    bool isLeft = false;
    bool isUp = false;
    bool isBottom = false;
    bool isFront = false;
    bool isBack = false;

    for (int i = 0; i < objectsVector.size(); i++) {
        glm::vec3 goPos = objectsVector.at(i).gameObject->getPosition();
        if(!isRight && position.x == goPos.x + 1) isRight = true;
        if(!isLeft && position.x == goPos.x - 1) isLeft = true;
        if(!isUp && position.y == goPos.y + 1) isUp = true;
        if(!isBottom && position.y == goPos.y - 1) isBottom = true;
        if(!isFront && position.z == goPos.z + 1) isFront = true;
        if(!isBack && position.z == goPos.z - 1) isBack = true;

        if(isRight && isLeft && isUp && isBottom && isFront && isBack) return false;
    }
    return true;
}

void StateController::addObject(GameObject* object) {
    tStateGameObject structObject = {
        object,
        true
    };
    objectsVector.push_back(structObject);
    std::sort (objectsVector.begin(), objectsVector.end(), objectSort);
}

std::vector<tStateGameObject> StateController::getObjects() {
    return objectsVector;
}

void StateController::update(float deltaTime) {
    for (int i = 0; i < objectsVector.size(); i++) {
        objectsVector.at(i).gameObject->update(this->window, deltaTime);
    }
}

void StateController::render(float deltaTime) {
    tLight light = {
        vec3(1.0f, 1.0f, 1.0f), // Color
        vec3(30.0f, 10.0f, 0.0f) // Position
    };

    glUseProgram(this->shader->getProgram());

	// Apply camera position
	shader->use("viewPos", this->camera->getPosition());
	shader->use("view", this->camera->getView());
	shader->use("projection", this->camera->getProjection());

    // Set object color
    shader->use("objectColor", vec3(1.0f));

	// Apply lighting
	shader->use("lightColor", light.color);
	shader->use("lightPosition", light.position);

    int lastType = 0;
    for (int i = 0; i < objectsVector.size(); i++) {
        tStateGameObject go = objectsVector.at(i);
        if(go.shouldRender) {
            int type = go.gameObject->getType();
            if(lastType != type) {
                lastType = type;
                go.gameObject->renderTexture(this->shader);
            }
            go.gameObject->render(this->shader);
        }
    }
}
