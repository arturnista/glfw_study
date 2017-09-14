// #pragma warning(disable:4996)
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
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

const char* readFile(string filename) {
	string fullFileData;
	ifstream openedFile(filename.c_str());
	if (openedFile.is_open()) {
		string line;
		while (getline(openedFile, line)) {
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

void compileShader(GLuint shader) {
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

typedef struct {
	GLuint VAO;
	int vertexCounter;
} gameObject;

gameObject createCube(glm::vec3 center, float size) {
	float halfSize = size / 2.0f;

	std::cout << center.x << '\n';
	std::cout << center.y << '\n';
	std::cout << center.z << '\n';

	GLfloat points[] = {
		// Positions           // Colors
		center.x + halfSize, center.y - halfSize, center.z + halfSize,   1.0f, 0.0f, 0.0f,  // Bottom right
		center.x - halfSize, center.y - halfSize, center.z + halfSize,   1.0f, 0.0f, 0.0f,  // Bottom left
		center.x + halfSize, center.y + halfSize, center.z + halfSize,   1.0f, 0.0f, 0.0f,  // Top right
		center.x - halfSize, center.y + halfSize, center.z + halfSize,   1.0f, 0.0f, 0.0f,  // Top left

		center.x + halfSize, center.y - halfSize, center.z - halfSize, 1.0f, 1.0f, 0.0f,  // Bottom right
		center.x - halfSize, center.y - halfSize, center.z - halfSize, 1.0f, 1.0f, 0.0f,  // Bottom left
		center.x + halfSize, center.y + halfSize, center.z - halfSize, 1.0f, 1.0f, 0.0f,  // Top right
		center.x - halfSize, center.y + halfSize, center.z - halfSize, 1.0f, 1.0f, 0.0f,  // Top left

		center.x + halfSize, center.y + halfSize, center.z - halfSize,   0.0f, 1.0f, 0.0f,  // Bottom right
		center.x + halfSize, center.y - halfSize, center.z - halfSize,   0.0f, 1.0f, 0.0f,  // Bottom left
		center.x + halfSize, center.y + halfSize, center.z + halfSize,   0.0f, 1.0f, 0.0f,  // Top right
		center.x + halfSize, center.y - halfSize, center.z + halfSize,   0.0f, 1.0f, 0.0f,  // Top left

		center.x - halfSize, center.y + halfSize, center.z - halfSize,  0.0f, 0.0f, 1.0f,  // Bottom right
		center.x - halfSize, center.y - halfSize, center.z - halfSize,  0.0f, 0.0f, 1.0f,  // Bottom left
		center.x - halfSize, center.y + halfSize, center.z + halfSize,  0.0f, 0.0f, 1.0f,  // Top righzt
		center.x - halfSize, center.y - halfSize, center.z + halfSize,  0.0f, 0.0f, 1.0f,  // Top leftas

		center.x + halfSize, center.y - halfSize, center.z - halfSize, 1.0f, 0.0f, 1.0f,  // Bottom right
		center.x - halfSize, center.y - halfSize, center.z - halfSize, 1.0f, 0.0f, 1.0f,  // Bottom left
		center.x + halfSize, center.y - halfSize, center.z + halfSize, 1.0f, 0.0f, 1.0f,  // Top right
		center.x - halfSize, center.y - halfSize, center.z + halfSize, 1.0f, 0.0f, 1.0f,  // Top left

	 	center.x + halfSize, center.y + halfSize, center.z - halfSize,  0.0f, 1.0f, 1.0f,  // Bottom right
		center.x - halfSize, center.y + halfSize, center.z - halfSize,  0.0f, 1.0f, 1.0f,  // Bottom left
		center.x + halfSize, center.y + halfSize, center.z + halfSize,  0.0f, 1.0f, 1.0f,  // Top right
		center.x - halfSize, center.y + halfSize, center.z + halfSize,  0.0f, 1.0f, 1.0f,  // Top left
	};
	GLuint indexArray[] = {
		0, 1, 3,
		3, 2, 0,

		4, 5, 7,
		7, 6, 4,

		8, 9, 11,
		11, 10, 8,

		12, 13, 15,
		15, 14, 12,

		16, 17, 19,
		19, 18, 16,

		20, 21, 23,
		23, 22, 20,
	};

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 25 * 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	GLuint EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	int vertexCounter = sizeof(indexArray) / sizeof(GLuint);

	return {VAO, vertexCounter};
}

int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	const float SCREEN_WIDTH = 800;
	const float SCREEN_HEIGHT = 400;


	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

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

	gameObject cube1 = createCube(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
	gameObject cube2 = createCube(glm::vec3(10.0f, 10.0f, 10.0f), 1.0f);

	const char* vertex_shader_program = readFile("shader.glsl");
	const char* fragment_shader_program = readFile("fragment.glsl");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader_program, NULL);
	compileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader_program, NULL);
	compileShader(fragmentShader);

	GLuint shaderProgramme = glCreateProgram();
	glAttachShader(shaderProgramme, vertexShader);
	glAttachShader(shaderProgramme, fragmentShader);
	glLinkProgram(shaderProgramme);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float rotateSpeed = 1;
	float distanceX = 0.0f;
	float distanceY = 0.0f;
	float distanceZ = 3.0f;

	float mouseDiffX = 0.0f;
	float mouseDiffY = 0.0f;

	double lastMouseX = 0.0f;
	double lastMouseY = 0.0f;

	float mouseSens = 5.0f;

	bool isFirstMouse = true;

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glUseProgram(shaderProgramme);

		// Create transformations
		glm::mat4 transform;
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * rotateSpeed, glm::vec3(0.0f, 0.0f, 1.0f));

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			distanceZ -= .1;
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			distanceZ += .1;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			distanceX -= .1;
		} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			distanceX += .1;
		}


		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		if (lastMouseX != mouseX) {
			if(!isFirstMouse) mouseDiffX += (mouseX - lastMouseX) / (SCREEN_WIDTH / mouseSens);
			isFirstMouse = false;
			lastMouseX = mouseX;
			std::cout << "X: " << mouseDiffX << '\n';
		}
		if (lastMouseY != mouseY) {
			if(!isFirstMouse) mouseDiffY += (mouseY - lastMouseY) / (SCREEN_HEIGHT / mouseSens);
			isFirstMouse = false;
			lastMouseY = mouseY;
			std::cout << "Y: " << mouseDiffY << '\n';
		}


		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// view = glm::translate(view, glm::vec3(distanceX, distanceY, -distanceZ));
		projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -distanceZ);
		glm::vec3 cameraTarget = glm::vec3(-mouseDiffX, -mouseDiffY, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		view = glm::lookAt(cameraPos,
				  		   cameraTarget,
				  		   cameraUp);

		int modelLoc = glGetUniformLocation(shaderProgramme, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgramme, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projectionLoc = glGetUniformLocation(shaderProgramme, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Get matrix's uniform location and set matrix
		GLint transformLoc = glGetUniformLocation(shaderProgramme, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(cube1.VAO);
		glDrawElements(GL_TRIANGLES, cube1.vertexCounter, GL_UNSIGNED_INT, 0);

		glBindVertexArray(cube2.VAO);
		glDrawElements(GL_TRIANGLES, cube2.vertexCounter, GL_UNSIGNED_INT, 0);

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((key == GLFW_KEY_ESCAPE || key == 'q' || key == 'Q') && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	// cout << xpos << '\n';
}
