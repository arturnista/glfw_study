#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__ 1

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <string>
#include <vector>

unsigned long hashNumber(long v);

unsigned long hashVec3(glm::vec3 v);

typedef struct {
	glm::vec3 color;
	glm::vec3 position;
} tLight;

typedef struct {
	std::vector<GLfloat> pointsVector;
	std::vector<GLuint> indexVector;
	std::vector<GLfloat> normalVector;
	std::vector<GLfloat> textureVector;
	bool hasTexture;
	int itemsPerPoint;
	GLfloat *points;
	int pointsCounter;
	GLuint *vertices;
	int verticesCounter;
} tObject;

#endif
