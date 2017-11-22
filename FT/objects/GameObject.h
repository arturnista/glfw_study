#ifndef __GAME_OBJECT_HEADER__
#define __GAME_OBJECT_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Shader.h"
#include "../Camera.h"
#include "../ResourcesManager.h"
#include "../common.h"

const int GO_TYPE_GRASS = 1;
const int GO_TYPE_BUNNY = 2;
const int GO_TYPE_DIRT = 3;
const int GO_TYPE_PLAYER = 4;
const int GO_TYPE_STONE = 5;
const int GO_TYPE_LAMP = 6;

using namespace std;
using namespace glm;

class GameObject {
protected:
	int type;
	ResourcesManager* resourcesManager;

	GLuint VAO;
	int vertexCounter;
	string textureName;
	bool hasTexture;
	tObject objectData;

	vec3 color;

	vec3 size;
    vec3 position;
	vec3 rotation;
    mat4 model;

	Shader* shader;
public:
	GameObject (ResourcesManager* rm, int type);

	GameObject (ResourcesManager* rm, int type, tObject objectData, float size, vec3 color);
	GameObject (ResourcesManager* rm, int type, tObject objectData, vec3 size, vec3 color);

	GameObject (ResourcesManager* rm, int type, string filename, float size, vec3 color);
    GameObject (ResourcesManager* rm, int type, string filename, vec3 size, vec3 color);

	int getType();

	GLuint getVAO();
	int getVertexCounter();

    virtual void update(GLFWwindow* window, float deltaTime);

	void setTextureName(string name);

    vec3 getPosition();
    float getPositionX();
    float getPositionY();
    float getPositionZ();
    void setPosition(vec3 position);
    void setPositionX(float value);
    void setPositionY(float value);
    void setPositionZ(float value);

    vec3 getColor();
	void setColor(vec3 color);

	tObject getObject();

    vec3 getSize();
	void setSize(float size);
    void setSize(vec3 size);

    void rotateX(float value);
    void rotateY(float value);
    void rotateZ(float value);

	virtual void render(Camera* camera, tLight light);
	virtual void render(Shader* shader);
    virtual void renderTexture(Shader* shader);
};

#endif
