#include "Shader.h"

Shader::Shader (string shaderName) {
    const char* vertex_shader_program = readFile("assets/shaders/" + shaderName + ".shader");
    const char* fragment_shader_program = readFile("assets/shaders/" + shaderName + ".frag");

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_program, NULL);
    compileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_program, NULL);
    compileShader(fragmentShader);

    shaderProgramme = glCreateProgram();
    glAttachShader(shaderProgramme, vertexShader);
    glAttachShader(shaderProgramme, fragmentShader);
    glLinkProgram(shaderProgramme);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader () {
    glDeleteProgram(shaderProgramme);
}


GLuint Shader::getProgram() {
    return shaderProgramme;
}


void Shader::compileShader(GLuint shader) {
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::use(string name, glm::vec3 value) {
    int lightColorLoc = glGetUniformLocation(shaderProgramme, name.c_str());
    glUniform3fv(lightColorLoc, 1, value_ptr(value));
}

void Shader::use(string name, glm::mat4 value) {
    int valueLoc = glGetUniformLocation(shaderProgramme, name.c_str());
    glUniformMatrix4fv(valueLoc, 1, GL_FALSE, value_ptr(value));
}

void Shader::use(string name, int value) {
	int valueLoc = glGetUniformLocation(shaderProgramme, name.c_str());
	glUniform1i(valueLoc, value);
}
