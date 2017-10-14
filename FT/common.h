#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__ 1

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

// std::string vec3ToString(glm::vec3 v) {
// 	std::string ret = std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
// 	return ret;
// }

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
