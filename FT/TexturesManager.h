#ifndef __TEXTURES_MANAGER_HEADER__
#define __TEXTURES_MANAGER_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./vendor/stb_image.h"

class TexturesManager {
private:
    std::map<std::string, unsigned int> textureMap;

    unsigned int processTexture(std::string filename);

public:
    TexturesManager ();

    unsigned int getTexture(std::string textureName);
};

#endif
