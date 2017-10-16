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

void StateController::jointObjects() {
    glm::vec3 lastPosition;
    for (int i = 0; i < objectsVector.size(); i++) {
        tStateGameObject stateGO = objectsVector.at(i);
        glm::vec3 position = stateGO.gameObject->getPosition();
        if(lastPosition != position) {
            lastPosition = position;
        }


    }
}

bool StateController::shouldRender(glm::vec3 pos) {
    unsigned long testPos = hashVec3(vec3(pos.x + 1, pos.y, pos.z));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    testPos = hashVec3(vec3(pos.x - 1, pos.y, pos.z));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y + 1, pos.z));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y - 1, pos.z));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y, pos.z + 1));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y, pos.z - 1));
    if( objectsVectorByPosition[testPos].gameObject == NULL ) return true;

    return false;
}

void StateController::prepareObjects() {
    for (int i = 0; i < objectsVector.size(); i++) {
        objectsVector[i].shouldRender = shouldRender(objectsVector[i].gameObject->getPosition());
    }
}

void StateController::addObject(GameObject* object) {
    tStateGameObject structObject = {
        object,
        true
    };
    objectsVector.push_back(structObject);
    unsigned long key = hashVec3(object->getPosition());
    objectsVectorByPosition[key] = structObject;

    this->prepareObjects();

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
