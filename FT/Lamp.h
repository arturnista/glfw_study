#ifndef __LAMP_HEADER__
#define __LAMP_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "readFiles.h"

class Lamp : public GameObject {
private:
    /* data */

public:
    Lamp (string filename, float size, vec3 color);

    void update(GLFWwindow* window, float deltaTime);
};

#endif
