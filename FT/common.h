#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__ 1

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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
	float itemsPerPoint;
	GLfloat *points;
	int pointsCounter;
	GLuint *vertices;
	int verticesCounter;
} tObject;

#endif
