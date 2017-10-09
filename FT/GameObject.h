#ifndef __GAME_OBJECT_HEADER__
#define __GAME_OBJECT_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "readFiles.h"
#include "common.h"

using namespace std;
using namespace glm;

class GameObject {
protected:
	GLuint VAO;
	int vertexCounter;

	float size;
    vec3 position;
	vec3 rotation;
    mat4 model;
public:
	GameObject ();
    GameObject (string filename, float size, vec3 color);

	GLuint getVAO();
	int getVertexCounter();

    virtual void update(GLFWwindow* window, float deltaTime);

    vec3 getPosition();
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    void setPosition(vec3 position);
    void setPositionX(float value);
    void setPositionY(float value);
    void setPositionZ(float value);

    float getSize();
    void setSize(float size);

    void rotateX(float value);
    void rotateY(float value);
    void rotateZ(float value);

    void render(Shader* shader, Camera* camera, tLight light);
};

#endif
