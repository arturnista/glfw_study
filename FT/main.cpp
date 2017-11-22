#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <time.h>       /* time */
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "StateController.h"

#include "Shader.h"
#include "ResourcesManager.h"
#include "Camera.h"
#include "./objects/Lamp.h"
#include "./objects/Player.h"
#include "./objects/Bunny.h"
#include "Map.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

Camera* camera;
StateController* stateController;
ResourcesManager* resourcesManager;
Map* mapCont;

int main() {
  	srand (time(NULL));
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	const float INITIAL_SCREEN_WIDTH = 1200;
	const float INITIAL_SCREEN_HEIGHT = 800;

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
    glfwSwapInterval(0); // Vsync off

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
    // glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    printf("\nPre GameLoop actions\n\n");

	glm::vec3 lightPosition = glm::vec3(30.0f, 10.0f, 0.0f);

    camera = new Camera(window);
    resourcesManager = new ResourcesManager();
    stateController = new StateController(window, camera, resourcesManager);
    mapCont = new Map(resourcesManager, stateController);

    Player* player = new Player(camera);
    stateController->addObject( player );
	Lamp* lampObject = new Lamp(resourcesManager);
	lampObject->setPosition(lightPosition);
    stateController->addObject( lampObject );

	float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
	float timePassed = 0.0f; // Time of last frame
    int frames = 0;

    printf("\nGameLoop actions\n\n");
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Compute the delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        timePassed += deltaTime;

        frames++;
        if(timePassed >= 2) {
            std::cout << "FPS:" << frames / 2.0f << '\t';
            std::cout << "Time: " << timePassed / frames << '\n';
            timePassed = 0;
            frames = 0;
        }

		/*
			Objects update
		*/
        mapCont->create(1);
        stateController->update(deltaTime);
        stateController->render(deltaTime);

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

    if(key == GLFW_KEY_F && action == GLFW_PRESS) {
        stateController->jointObjects();
    }

    if(key == GLFW_KEY_V && action == GLFW_PRESS) {
        stateController->jointObjectsNEW();
    }

    if(key == GLFW_KEY_R && action == GLFW_PRESS) {
        stateController->jointObjects(true);
    }

    if(key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
        stateController->setY(stateController->getY() - 1);
    }

    if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
        stateController->setY(stateController->getY() + 1);
    }

    if(key == GLFW_KEY_G && action == GLFW_PRESS) {
        float x = floor( rand() % 25 );
        float y = floor( rand() % 25 );
        float z = floor( rand() % 5 );
        mapCont->breakItem(glm::vec3(x, y, z));
    }
}

// Is called whenever the mouse moves
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
}
