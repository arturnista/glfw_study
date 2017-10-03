#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__ 1

typedef struct {
	GLuint VAO;
	int vertexCounter;
	float size;
	glm::vec3 position;
	float angle_front;
	float angle_side;
	float angle_back;
} gameObject;

#endif
