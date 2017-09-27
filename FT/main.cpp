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
using namespace glm;
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

vec3 splitLine(string line) {
	string::size_type sz;
	string strNumber = "";
	int idx = 0;
	vec3 result;

	bool digitFounded = false;

	for (size_t i = 2; i < line.length(); i++) {
		char character = line.at(i);
		if(!digitFounded) {
			if(character == ' ') continue;
			digitFounded = true;
		}

		if(character != ' ') {
			strNumber += character;
		} else  {
			float number = stof(strNumber, &sz);
			strNumber = "";
			if(idx == 0) result.x = number;
			else if(idx == 1) result.y = number;
			else if(idx == 2) result.z = number;
			idx++;
		}
	}
	if(idx == 2) {
		float finalNumber = stof(strNumber, &sz);
		result.z = finalNumber;
	}
	return result;
}

gameObject readObject(string filename, float size, vec3 offset) {
	std::vector<GLfloat> pointsVector = {};
    std::vector<GLuint> indexVector = {};

	ifstream openedFile(filename.c_str());
	if (openedFile.is_open()) {
		string line;
		while (getline(openedFile, line)) {
			char first = '#';
			if(line.length() > 0) first = line.at(0);

			if(first == 'v') {
				vec3 point = splitLine(line);
				pointsVector.push_back(point.x + offset.x);
				pointsVector.push_back(point.y + offset.y);
				pointsVector.push_back(point.z + offset.z);
			}
			if(first == 'f') {
				vec3 index = splitLine(line);
				indexVector.push_back(index.x);
				indexVector.push_back(index.y);
				indexVector.push_back(index.z);
			}
		}
		openedFile.close();
	}

	int pointsCounter = pointsVector.size();
	int vertexCounter = indexVector.size();

	cout << "vertex count of " << filename << " = " << (pointsCounter / 3) << '\n';
	cout << "face count of " << filename << " = " << (vertexCounter / 3) << '\n';

	GLfloat *points = new GLfloat[pointsCounter];
	copy(pointsVector.begin(), pointsVector.end(), points);
	if(size != 1) for (size_t i = 0; i < pointsCounter; i++) points[i] = points[i] * size;

	GLuint *indexArray = new GLuint[vertexCounter];
	copy(indexVector.begin(), indexVector.end(), indexArray);
	for (size_t i = 0; i < vertexCounter; i++) indexArray[i] = indexArray[i] - 1;

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointsCounter * sizeof(GLfloat), points, GL_STATIC_DRAW);
	// With colors
	// glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(GLfloat), points, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Positions without colors
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Positions
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	// glEnableVertexAttribArray(0);

	// Colors
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	// glEnableVertexAttribArray(1);

	GLuint EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertexCounter, indexArray, GL_STATIC_DRAW);

	return {VAO, vertexCounter};
}

gameObject readObject(string filename, float size = 1) {
	return readObject(filename, size, vec3(0.0f, 0.0f, 0.0f));
}

int main() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	glEnable(GL_CULL_FACE);
	const float INITIAL_SCREEN_WIDTH = 800;
	const float INITIAL_SCREEN_HEIGHT = 600;

	GLFWwindow* window = glfwCreateWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	int CUBES_COUNTER = 2;
	gameObject *cubes = new gameObject[CUBES_COUNTER];
	cubes[0] = readObject("./Objects/bunny.obj", 10);
	cubes[1] = readObject("./Objects/cube.obj", 1, vec3(10, 0, 0));

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

	double lastMouseX = 0.0f;
	double lastMouseY = 0.0f;

	float mouseSens = 1.3f;

	bool isFirstMouse = true;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	vec3 cameraPos = vec3(0.0f, 0.0f, -3.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, 0.0f);

	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraDirection = normalize(cameraPos - cameraTarget);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraRight = normalize(cross(up, cameraDirection));
	vec3 cameraUp = cross(cameraDirection, cameraRight);

	float pitch = 0;
	float yaw = 0;

	while (!glfwWindowShouldClose(window)) {
		// Updates the screen size
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		float SCREEN_WIDTH = width;
		float SCREEN_HEIGHT = height;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glUseProgram(shaderProgramme);

		// Compute the delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Get the cursor position
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Compute the mouse position
		// Uses the screen center as reference
		float mouseOffsetX = deltaTime * mouseSens * (SCREEN_WIDTH / 2 - mouseX);
		float mouseOffsetY = deltaTime * mouseSens * (SCREEN_HEIGHT / 2 - mouseY);

		pitch += mouseOffsetY;
		yaw += -mouseOffsetX;
		if(pitch > 89.0f) pitch =  89.0f;
		if(pitch < -89.0f) pitch = -89.0f;

		// std::cout << yaw << '\n';

		vec3 lookingDirection = vec3(
			cos(radians(pitch)) * cos(radians(yaw)),
			sin(radians(pitch)),
			cos(radians(pitch)) * sin(radians(yaw))
		);

		cameraFront = normalize(lookingDirection);

		float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		// Create and compute the model view projection Camera's components
		mat4 model;
		mat4 view;
		mat4 projection;

		model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgramme, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgramme, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
		int projectionLoc = glGetUniformLocation(shaderProgramme, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

		// Draw each component
		for (size_t i = 0; i < CUBES_COUNTER; i++) {
			glBindVertexArray(cubes[i].VAO);
			glDrawElements(GL_TRIANGLES, cubes[i].vertexCounter, GL_UNSIGNED_INT, 0);
		}

		// Reset the vertex bind
		glBindVertexArray(0);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		// Set the cursor to the middle of the screen
		glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
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
