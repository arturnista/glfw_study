#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const char* readFile(string filename) {
	string fullFileData;
	ifstream openedFile (filename.c_str());
	if (openedFile.is_open()) {
		string line;
		while ( getline (openedFile, line) ) {
			fullFileData += line + '\n';
		}
		openedFile.close();
	}

	// Copy the string
	char * writable = new char[fullFileData.size() + 1];
	std::copy(fullFileData.begin(), fullFileData.end(), writable);
	writable[fullFileData.size()] = '\0';

	return writable;
}

int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	GLfloat points[] = {
		// Positions           // Colors
		 0.5f, -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,  // Bottom right
		-0.5f, -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,  // Bottom left
		 0.5f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,  // Top right
		-0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 1.0f,  // Top left
		 0.8f,  0.0f,  0.0f,   0.0f, 1.0f, 1.0f,  // Mid right
		-0.8f,  0.0f,  0.0f,   1.0f, 1.0f, 0.0f,  // Mid left
	};
	GLuint indexArray[] = {
		0, 1, 3,
		3, 2, 0,
		0, 2, 4,
		3, 1, 5,
	};

	GLuint VBO_LEFT = 0;
	glGenBuffers(1, &VBO_LEFT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);
	glBufferData(GL_ARRAY_BUFFER, 6 * 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	GLuint VAO_LEFT = 0;
	glGenVertexArrays(1, &VAO_LEFT);
	glBindVertexArray(VAO_LEFT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	GLuint EBO_LEFT = 0;
	glGenBuffers(1, &EBO_LEFT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_LEFT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);


	const char* vertex_shader_program = readFile("shader.glsl");
	const char* fragment_shader_program = readFile("fragment.glsl");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader_program, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_program, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgramme = glCreateProgram();
	glAttachShader(shaderProgramme, vertexShader);
	glAttachShader(shaderProgramme, fragmentShader);
	glLinkProgram(shaderProgramme);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int vertexCounter = sizeof(indexArray) / sizeof(GLuint);
	float rotateSpeed = 0;

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glUseProgram(shaderProgramme);

		// Create transformations
		glm::mat4 transform;
		// transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * rotateSpeed, glm::vec3(0.0f, 0.0f, 1.0f));
		rotateSpeed += .01;

		// Get matrix's uniform location and set matrix
		GLint transformLoc = glGetUniformLocation(shaderProgramme, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO_LEFT);
		glDrawElements(GL_TRIANGLES, vertexCounter, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

						  // close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
