#include "StateController.h"

struct myclass {
    inline bool operator() (tStateGameObject struct1, tStateGameObject struct2) {
        return struct1.gameObject->getType() < struct2.gameObject->getType();
    }
} objectSort;


StateController::StateController(GLFWwindow* window, Camera* camera, ResourcesManager* rm) {
    objectsVector = {};
    this->window = window;
    this->camera = camera;
    this->resourcesManager = rm;
    this->shader = new Shader("texture");

    this->groundGameObject = NULL;

    this->light = rm->getLight();
}

void StateController::jointObjects(bool reset) {

    tObject object = objectsVector.at(1).gameObject->getObject();
    for (size_t i = 2; i < objectsVector.size(); i++) {
        GameObject* tempGameObject = objectsVector.at(i).gameObject;
        if(tempGameObject->getType() == GO_TYPE_GROUND) {
            object = this->resourcesManager->combineObjects( object, tempGameObject->getObject(), tempGameObject->getPosition() );
        }
    }

    GameObject* go = new GameObject(this->resourcesManager, GO_TYPE_GROUND, object, 1.0f, vec3(1.0f));
    go->setTextureName("objects");
    objectsToRenderList.clear();
    objectsToRenderList.push_back({ go, true });

    this->groundObject = object;

    delete this->groundGameObject;
    this->groundGameObject = go;
}

void StateController::reset() {
    if(this->groundGameObject != NULL) {
        delete this->groundGameObject;
        this->groundGameObject = NULL;
    }
    this->groundObject = {};

    for (size_t i = 0; i < objectsVector.size(); i++) {
        delete objectsVector.at(i).gameObject;
        objectsVector.at(i).gameObject = NULL;
    }

    objectsVector.clear();
    objectsToRenderList.clear();
    objectsMapByPosition.clear();
}

void StateController::addObject(GameObject* object) {
    int type = object->getType();

    tStateGameObject structObject = {
        object,
        type != GO_TYPE_PLAYER
    };

    unsigned long key = hashVec3(object->getPosition());
    if(type == GO_TYPE_GROUND) {
        objectsMapByPosition[key] = GROUND_INDEX;

        if(this->groundGameObject == NULL) {
            this->groundObject = object->getObject();
        } else {
            this->groundObject = this->resourcesManager->combineObjects( this->groundObject, object->getObject(), object->getPosition() );
        }

        delete object;
        delete this->groundGameObject;

        GameObject* go = new GameObject(this->resourcesManager, GO_TYPE_GROUND, this->groundObject, 1.0f, vec3(1.0f));
        go->setTextureName("objects");
        this->groundGameObject = go;
    } else {
        objectsVector.push_back(structObject);
        objectsToRenderList.push_back( structObject );

        objectsMapByPosition[key] = objectsVector.size() - 1;

        if(type == GO_TYPE_PLAYER) {
            playerIndex = objectsVector.size() - 1;
        }
    }
}

std::vector<tStateGameObject> StateController::getObjects() {
    return objectsVector;
}

GameObject* StateController::getPlayer() {
    return objectsVector.at(this->playerIndex).gameObject;
}

bool StateController::hasGroundAtPosition(glm::vec3 position) {
    int idx = objectsMapByPosition[ hashVec3(position) ];
    return idx == GROUND_INDEX;
}

void StateController::update(float deltaTime) {
    for (size_t i = 0; i < objectsVector.size(); i++) {
        GameObject* go = objectsVector.at(i).gameObject;
        if(go == NULL) {
            continue;
        }
        if(go->getType() != GO_TYPE_GROUND) {
            go->update(this->window, deltaTime);
        }
    }
}

void StateController::render(float deltaTime) {
    glUseProgram(this->shader->getProgram());

	// Apply camera position
	shader->use("viewPos", this->camera->getPosition());
	shader->use("view", this->camera->getView());
	shader->use("projection", this->camera->getProjection());

    // Set object color
    // shader->use("objectColor", vec3(1.0f));

	// Apply lighting
	shader->use("lightColor", this->light.color);
	shader->use("lightPosition", this->light.position);

    int lastType = 0;
    if(groundGameObject != NULL) {
        groundGameObject->renderTexture(this->shader);
        groundGameObject->render(this->shader);
    }

    std::list<tStateGameObject>::const_iterator renderIterator;
    for (renderIterator = objectsToRenderList.begin(); renderIterator != objectsToRenderList.end(); ++renderIterator) {
        tStateGameObject go = *renderIterator;
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
