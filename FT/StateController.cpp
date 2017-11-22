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

    this->yrender = 0;
}

void StateController::setY(int y) {
    this->yrender = y;
}

int StateController::getY() {
    return this->yrender;
}

void StateController::jointObjects(bool reset) {
    std::list<tStateGameObject> objectsToRenderListToAnalyse;
    objectsToRenderList.clear();
    for (size_t i = 0; i < objectsVector.size(); i++) {
        if(!objectsVector.at(i).shouldRender) continue;
        int type = objectsVector.at(i).gameObject->getType();
        // Player should not be rendered
        if(type != GO_TYPE_PLAYER) {
            objectsToRenderList.push_back( objectsVector.at(i) );
            objectsToRenderListToAnalyse.push_back( objectsVector.at(i) );
        }
    }

    if(reset) return;
    int counter = 0;
    while(!objectsToRenderListToAnalyse.empty()) {
        counter++;
        std::list<tStateGameObject> valid;
        std::list<tStateGameObject> next = {  };
        std::list<tStateGameObject> nextPromise = { objectsToRenderListToAnalyse.front() };
        // std::list<tStateGameObject> nextPromise = { objectsVector[10] };

        int compareType = nextPromise.front().gameObject->getType();
        glm::vec3 initialPos = nextPromise.front().gameObject->getPosition();
        float minnorX = initialPos.x;
        float minnorY = initialPos.y;
        float minnorZ = initialPos.z;

        float biggerX = initialPos.x;
        float biggerY = initialPos.y;
        float biggerZ = initialPos.z;

        bool running = true;

        while (running) {
            std::copy( nextPromise.begin(), nextPromise.end(), std::back_inserter( next ) );
            nextPromise.clear();

            while (!next.empty()) {
                // Get the first object and remove it from the list
                tStateGameObject frontObj = next.front();
                valid.push_back( frontObj );
                next.pop_front();

                // frontObj.gameObject->setColor( vec3(1.0f, 0.0f, 0.0f) );

                // Add the new itens from to valid list
                glm::vec3 pos = frontObj.gameObject->getPosition();
                int xCompare = pos.x - initialPos.x;
                int zCompare = pos.z - initialPos.z;

                tHash testPos;
                tStateGameObject objToAdd;
                int testIndex;
                if(xCompare >= zCompare) {
                    testPos = hashVec3(vec3(pos.x + 1, pos.y, pos.z));
                    testIndex = objectsMapByPosition[testPos];
                    objToAdd = objectsVector.at(testIndex);

                    if(objectsVector.size() > testIndex && compareType == objToAdd.gameObject->getType()) {
                        nextPromise.push_back( objToAdd );
                    } else {
                        running = false;
                    }
                }
                if(xCompare <= zCompare) {
                    testPos = hashVec3(vec3(pos.x, pos.y, pos.z + 1));
                    testIndex = objectsMapByPosition[testPos];
                    objToAdd = objectsVector.at(testIndex);

                    if(objectsVector.size() > testIndex && compareType == objToAdd.gameObject->getType()) {
                        nextPromise.push_back( objToAdd );
                    } else {
                        running = false;
                    }
                }
                if(xCompare == zCompare) {
                    testPos = hashVec3(vec3(pos.x + 1, pos.y, pos.z + 1));
                    testIndex = objectsMapByPosition[testPos];
                    objToAdd = objectsVector.at(testIndex);

                    if(objectsVector.size() > testIndex && compareType == objToAdd.gameObject->getType()) {
                        nextPromise.push_back( objToAdd );
                    } else {
                        running = false;
                    }
                }
            }
        }

        float squadSize = sqrt(valid.size());
        if(squadSize == 1.0f) {
            objectsToRenderListToAnalyse.remove( valid.front() );
            continue;
        }

        std::list<tStateGameObject>::const_iterator validIteractor;
        for (validIteractor = valid.begin(); validIteractor != valid.end(); ++validIteractor) {
            tStateGameObject validSGo = *validIteractor;

            vec3 pos = validSGo.gameObject->getPosition();
            if(biggerX < pos.x) biggerX = pos.x;
            if(biggerY < pos.y) biggerY = pos.y;
            if(biggerZ < pos.z) biggerZ = pos.z;

            if(minnorX > pos.x) minnorX = pos.x;
            if(minnorY > pos.y) minnorY = pos.y;
            if(minnorZ > pos.z) minnorZ = pos.z;

            objectsToRenderList.remove( validSGo );
            objectsToRenderListToAnalyse.remove( validSGo );
        }
        glm::vec3 center = vec3(
            ( (biggerX - minnorX) / 2 ) + minnorX,
            ( (biggerY - minnorY) / 2 ) + minnorY,
            ( (biggerZ - minnorZ) / 2 ) + minnorZ
        );

        GameObject* compactGo;
        if(compareType == GO_TYPE_GRASS) {
            compactGo = new Grass(resourcesManager);
        } else  if(compareType == GO_TYPE_BUNNY) {
            compactGo = new Bunny(resourcesManager);
        } else  if(compareType == GO_TYPE_DIRT) {
            compactGo = new Dirt(resourcesManager);
        } else  if(compareType == GO_TYPE_STONE) {
            compactGo = new Stone(resourcesManager);
        } else  if(compareType == GO_TYPE_LAMP) {
            compactGo = new Lamp(resourcesManager);
        }

        compactGo->setSize(glm::vec3(squadSize, 1.0f, squadSize));
        compactGo->setPosition(center);
        objectsToRenderList.push_back( { compactGo, true } );
    }

}

void StateController::jointObjectsNEW(bool reset) {

    tObject object = objectsVector.at(2).gameObject->getObject();
    for (size_t i = 3; i < objectsVector.size(); i++) {
        GameObject* tempGameObject = objectsVector.at(i).gameObject;
        object = this->resourcesManager->combineObjects( object, tempGameObject->getObject(), tempGameObject->getPosition() );
    }
    std::cout << "itemsPerPoint " << object.itemsPerPoint << '\n';
    std::cout << "pointsCounter " << object.pointsCounter << '\n';
    std::cout << "verticesCounter " << object.verticesCounter << '\n';

    GameObject* go = new GameObject(this->resourcesManager, GO_TYPE_STONE, object, 1.0f, vec3(0.0f));
    go->setTextureName("stone");
    objectsToRenderList.clear();
    objectsToRenderList.push_back({ go, true });
}

bool StateController::shouldRender(glm::vec3 pos) {
    unsigned long testPos = hashVec3(vec3(pos.x + 1, pos.y, pos.z));
    int testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

    testPos = hashVec3(vec3(pos.x - 1, pos.y, pos.z));
    testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y + 1, pos.z));
    testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y - 1, pos.z));
    testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y, pos.z + 1));
    testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

    testPos = hashVec3(vec3(pos.x, pos.y, pos.z - 1));
    testIndex = objectsMapByPosition[testPos];
    if( objectsVector.size() > testIndex ) return true;

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
    int type = structObject.gameObject->getType();
    if(type != GO_TYPE_PLAYER) {
        objectsToRenderList.push_back( structObject );
        unsigned long key = hashVec3(object->getPosition());
        objectsMapByPosition[key] = objectsVector.size() - 1;
        this->prepareObjects();
        // this->jointObjects();
    }

    // std::sort (objectsVector.begin(), objectsVector.end(), objectSort);
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
    // shader->use("objectColor", vec3(1.0f));

	// Apply lighting
	shader->use("lightColor", light.color);
	shader->use("lightPosition", light.position);

    int lastType = 0;
    std::list<tStateGameObject>::const_iterator renderIterator;
    for (renderIterator = objectsToRenderList.begin(); renderIterator != objectsToRenderList.end(); ++renderIterator) {
        tStateGameObject go = *renderIterator;
        if(go.shouldRender) {
            if(go.gameObject->getPosition().y > this->yrender) continue;

            int type = go.gameObject->getType();
            if(lastType != type) {
                lastType = type;
                go.gameObject->renderTexture(this->shader);
            }

            go.gameObject->render(this->shader);
        }
    }
}
