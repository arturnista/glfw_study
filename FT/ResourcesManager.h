#ifndef __RESOURCES_MANAGER_HEADER__
#define __RESOURCES_MANAGER_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "./utils/readFiles.h"
#include "./vendor/stb_image.h"
#include "./vendor/json.h"

using tJson = nlohmann::json;

class ResourcesManager {
private:
    std::map<std::string, unsigned int> textureMap;
    std::map<std::string, tObject> objectMap;
    tLight light;

    unsigned int processTexture(std::string filename);
    tObject processObjectFile(std::string filename);

public:
    ResourcesManager ();

    tObject combineObjects(tObject object1, tObject object2, glm::vec3 offset);

    unsigned int getTexture(std::string textureName);
    tObject getObject(std::string objectName);
    tLight getLight();
};

#endif
