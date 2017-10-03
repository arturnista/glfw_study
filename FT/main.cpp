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
#include <time.h>       /* time */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"
#include "readFiles.h"


using namespace std;
using namespace glm;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

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

int main() {
  	srand (time(NULL));
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

	int GAME_OBJECTS_COUNTER = 2;
	gameObject *gameObjects = new gameObject[GAME_OBJECTS_COUNTER];
	gameObjects[0] = readObjectFile("./Objects/bunny.obj", 10);
	gameObjects[0].position = vec3(0, 0, 0);
	gameObjects[1] = readObjectFile("./Objects/cube.obj", 1);
	gameObjects[0].position = vec3(1, 1, 0);

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

	float rotateSpeed = .1;
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

		vec3 lookingDirection = vec3(
			cos(radians(pitch)) * cos(radians(yaw)),
			sin(radians(pitch)),
			cos(radians(pitch)) * sin(radians(yaw))
		);

		cameraFront = normalize(lookingDirection);

		float cameraSpeed = 10.0f * deltaTime;
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

		/*
			Bunny movement, rotation and scale
		*/

		float bunnySpeed = 5.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			gameObjects[0].position.z += bunnySpeed;
		} else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			gameObjects[0].position.z -= bunnySpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
			gameObjects[0].position.x -= bunnySpeed;
		} else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
			gameObjects[0].position.x += bunnySpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			gameObjects[0].position.y += bunnySpeed;
		} else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
			gameObjects[0].position.y -= bunnySpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			gameObjects[0].angle_front -= 1.0f * deltaTime;
		} else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			gameObjects[0].angle_front += 1.0f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			gameObjects[0].angle_side -= 1.0f * deltaTime;
		} else if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			gameObjects[0].angle_side += 1.0f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			gameObjects[0].angle_back -= 1.0f * deltaTime;
		} else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
			gameObjects[0].angle_back += 1.0f * deltaTime;
		}

		if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
			gameObjects[0].size -= 5.0f * deltaTime;
			if(gameObjects[0].size < 0) gameObjects[0].size = 0;
		} else if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
			gameObjects[0].size += 5.0f * deltaTime;
		}

		// Create and compute the model view projection Camera's components
		mat4 model;
		mat4 view;
		mat4 projection;

		model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
		view = lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = perspective(radians(45.0f), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

		for (size_t i = 0; i < GAME_OBJECTS_COUNTER; i++) {
			glUseProgram(shaderProgramme);

			// Apply the model, view and projection on the shader created
			int modelLoc = glGetUniformLocation(shaderProgramme, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
			int viewLoc = glGetUniformLocation(shaderProgramme, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
			int projectionLoc = glGetUniformLocation(shaderProgramme, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(projection));

			// Compute the transform for each object
			glm::mat4 transform;
			transform = translate(transform, gameObjects[i].position);

			transform = rotate(transform, gameObjects[i].angle_front, vec3(0.0f, 0.0f, 1.0f));
			transform = rotate(transform, gameObjects[i].angle_side, vec3(0.0f, 1.0f, 0.0f));
			transform = rotate(transform, gameObjects[i].angle_back, vec3(1.0f, 0.0f, 0.0f));

			transform = scale(transform, vec3(gameObjects[i].size, gameObjects[i].size, gameObjects[i].size));

			// Apply the transform object
			GLint transformLoc = glGetUniformLocation(shaderProgramme, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));

			glBindVertexArray(gameObjects[i].VAO);
			glDrawElements(GL_TRIANGLES, gameObjects[i].vertexCounter, GL_UNSIGNED_INT, 0);
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
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	// cout << xpos << '\n';
}
