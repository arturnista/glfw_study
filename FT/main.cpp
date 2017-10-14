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
#include "ResourcesManager.h"
#include "Camera.h"
#include "Lamp.h"
#include "Player.h"
#include "Bunny.h"
#include "Grass.h"
#include "Dirt.h"
#include "Stone.h"

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
    // glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    printf("\nPre GameLoop actions\n\n");

	vec3 lightPosition = vec3(30.0f, 10.0f, 0.0f);

    Camera* camera = new Camera(window);
    StateController* stateController = new StateController(window, camera);
    ResourcesManager* resourcesManager = new ResourcesManager();

    // Bunny* bunnyObject = new Bunny(resourcesManager);
    // bunnyObject->setPosition(vec3(0, 0, 0));
    // stateController->addObject( bunnyObject );

    Player* player = new Player(camera);
    stateController->addObject( player );
	Lamp* lampObject = new Lamp(resourcesManager);
	lampObject->setPosition(lightPosition);
    stateController->addObject( lampObject );

    int size = 40;
    int height = 10;
    float fullBlocks = size * size * height;
    int blocksCreated = 0;
    int perc;
    int lastPercShow = 0;
    for (size_t xInc = 0; xInc < size; xInc++) {
        for (size_t yInc = 0; yInc < height; yInc++) {
            for (size_t zInc = 0; zInc < size; zInc++) {
                blocksCreated++;
                perc = floor((blocksCreated / fullBlocks) * 100);
                if(perc != lastPercShow && perc % 10 == 0) {
                    std::cout << "Loading: " << perc << "%" << '\n';
                    lastPercShow = perc;
                }

                if(yInc == 0) {
                	Grass* grassObject = new Grass(resourcesManager);
                	grassObject->setPosition(vec3(xInc, yInc * -1.0f, zInc));
                    stateController->addObject( grassObject );
                } else if(yInc < 3) {
                    Dirt* dirtObject = new Dirt(resourcesManager);
                    dirtObject->setPosition(vec3(xInc, yInc * -1.0f, zInc));
                    stateController->addObject( dirtObject );
                } else {
                    Stone* stoneObject = new Stone(resourcesManager);
                    stoneObject->setPosition(vec3(xInc, yInc * -1.0f, zInc));
                    stateController->addObject( stoneObject );
                }
            }
        }
    }

	float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
	float timePassed = 0.0f; // Time of last frame

    printf("\nGameLoop actions\n\n");

    stateController->prepareObjects();
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Compute the delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        timePassed += deltaTime;

        if(timePassed >= 2) {
            std::cout << 1 / deltaTime << '\n';
            timePassed = 0;
        }

		/*
			Objects update
		*/
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
}

// Is called whenever the mouse moves
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
}
