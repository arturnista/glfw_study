#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

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

	float points[] = {
		-0.3f,  0.3f,  0.0f,
		0.0f, -0.3f,  0.0f,
		-0.6f, -0.6f,  0.0f,
	};

	GLuint VBO_LEFT = 0;
	glGenBuffers(1, &VBO_LEFT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint VAO_LEFT = 0;
	glGenVertexArrays(1, &VAO_LEFT);
	glBindVertexArray(VAO_LEFT);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	float points1[] = {
		0.3f,  0.3f,  0.0f,
		0.6f, -0.3f,  0.0f,
		0.0f, -0.3f,  0.0f
	};

	GLuint VBO_RIGHT = 0;
	glGenBuffers(1, &VBO_RIGHT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RIGHT);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points1, GL_STATIC_DRAW);

	GLuint VAO_RIGHT = 0;
	glGenVertexArrays(1, &VAO_RIGHT);
	glBindVertexArray(VAO_RIGHT);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RIGHT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgramme);

		glBindVertexArray(VAO_LEFT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// float timeValue = glfwGetTime();
		// float greenValue = sin(timeValue) / 2.0f + 0.5f;
		// int vertexColorLocation = glGetUniformLocation(shaderProgramme, "ourColor");
		// glUniform4f(vertexColorLocation, 0.0f, greenValue, 1 - greenValue, 1.0f);

		glBindVertexArray(VAO_RIGHT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

						  // close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
