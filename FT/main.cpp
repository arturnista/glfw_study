#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

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

#include "StateController.h"

#include "Shader.h"
#include "Camera.h"
#include "Lamp.h"
#include "Player.h"
#include "Bunny.h"
#include "Cube.h"

using namespace std;
using namespace glm;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    printf("\nPre GameLoop actions\n\n");

	vec3 lightPosition = vec3(5.0f, 5.0f, 0.0f);

    Camera* camera = new Camera(window);
    StateController* stateController = new StateController(window, camera);

    Player* player = new Player(camera);
	Bunny* bunnyObject = new Bunny();
	bunnyObject->setPosition(vec3(0, 0, 0));
	Cube* cubeObject = new Cube();
	cubeObject->setPosition(vec3(0, 2, 2));
	Lamp* lampObject = new Lamp();
	lampObject->setPosition(lightPosition);

    stateController->addObject( player );
    stateController->addObject( bunnyObject );
    stateController->addObject( lampObject );
    stateController->addObject( cubeObject );

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

    tLight light = {
        vec3(1.0f, 1.0f, 1.0f),
        lightPosition
    };

    printf("\nGameLoop actions\n\n");
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Compute the delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
			Objects update
		*/
        vector<GameObject*> gameObjects = stateController->getObjects();
        for (int i = 0; i < gameObjects.size(); i++) gameObjects.at(i)->update(window, deltaTime);
        for (int i = 0; i < gameObjects.size(); i++) gameObjects.at(i)->render(camera, light);

		// Reset the vertex bind
		glBindVertexArray(0);

		// Update other events like input handling
		glfwPollEvents();

		// Put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// Close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// Is called whenever the mouse moves
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
}
