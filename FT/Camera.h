#ifndef __CAMERA_HEADER__
#define __CAMERA_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera {
private:
    vec3 position;
    vec3 front;

    vec3 target;
    vec3 direction;
    vec3 right;
    vec3 up;

    GLFWwindow* window;
    mat4 projection;
public:
    Camera (GLFWwindow* window);

    vec3 getUp();

    vec3 getPosition();
    void setPosition(vec3 position);

    vec3 getFront();
    void setFront(vec3 front);

    void resetProjection();

    mat4 getView();
    mat4 getProjection();
};

#endif
