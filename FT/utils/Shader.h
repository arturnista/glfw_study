#ifndef __SHADER_HEADER__
#define __SHADER_HEADER__ 1

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "readFiles.h"

class Shader {
private:
    GLuint shaderProgramme;

    void compileShader(GLuint shader);
public:
    Shader (string shaderName);

    GLuint getProgram();

    void use(string name, glm::vec3 value);
	void use(string name, glm::mat4 value);
	void use(string name, int value);
};

#endif
