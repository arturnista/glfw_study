#include "Bunny.h"

Bunny::Bunny(string filename, float size, vec3 color) : GameObject(filename, size, color) {}

void Bunny::update(GLFWwindow* window, float deltaTime) {
    /*
        Bunny movement, rotation and scale
    */
    float bunnySpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        this->setPositionZ(this->getPositionZ() + bunnySpeed);
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        this->setPositionZ(this->getPositionZ() - bunnySpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        this->setPositionX(this->getPositionX() + bunnySpeed);
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        this->setPositionX(this->getPositionX() - bunnySpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        this->setPositionY(this->getPositionY() + bunnySpeed);
    } else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        this->setPositionY(this->getPositionY() - bunnySpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        this->rotateX(-1.0f * deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        this->rotateX(+1.0f * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        this->rotateY(-1.0f * deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        this->rotateY(+1.0f * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        this->rotateZ(-1.0f * deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        this->rotateZ(+1.0f * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        float size = this->getSize() - (5.0f * deltaTime);
        if(size < 0) size = 0;
        this->setSize(size);
    } else if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        float size = this->getSize() + (5.0f * deltaTime);
        this->setSize(size);
    }
}
