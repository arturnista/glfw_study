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
#include <cstdlib>
#include "./vendor/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "StateController.h"

#include "Shader.h"
#include "ResourcesManager.h"
#include "Camera.h"
#include "LevelController.h"
#include "./objects/Lamp.h"
#include "./objects/Player.h"
#include "./objects/Enemy.h"
#include "./utils/readFiles.h"
#include "Map.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

Camera* camera;
StateController* stateController;
ResourcesManager* resourcesManager;
Map* mapCont;
LevelController* levelController;

bool setNextLevel = false;

void setupGUI(GLFWwindow* window);
void rendererFn(ImDrawData* draw_data);

const int START = 0;
const int PLAYGROUND = 1;
const int GAME = 2;
const int RESTART = 3;

int gameState = START;

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
    setupGUI(window);

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

    glEnable(GL_CULL_FACE);

    printf("\nPre GameLoop actions\n\n");

    camera = new Camera(window);
    resourcesManager = new ResourcesManager();
    stateController = new StateController(window, camera, resourcesManager);
    mapCont = new Map(resourcesManager, stateController);

    levelController = new LevelController(window, resourcesManager, stateController, mapCont, camera);
    int tpPlaygroundIndex = levelController->home();

    // mapCont->create();

	float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = glfwGetTime(); // Time of last frame
	float timePassed = 0.0f; // Time of last frame
    int frames = 0;

    printf("\nGameLoop actions\n\n");

    // while (false) {
	while (!glfwWindowShouldClose(window)) {

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glm::vec3 playerPos = stateController->getPlayer()->getPosition();
        if(playerPos.y < -5) {
            levelController->restart();
        }

        if(gameState == START) {
            glm::vec3 tpPos = stateController->getGameObject(tpPlaygroundIndex)->getPosition();
            float tpDistance = distance(playerPos, tpPos);
            if(tpDistance < 1) {
                gameState = PLAYGROUND;
                levelController->playground();
                continue;
            }

            glm::vec3 gamePos = stateController->getGameObject(tpPlaygroundIndex + 1)->getPosition();
            float gameDistance = distance(playerPos, gamePos);
            if(gameDistance < 1) {
                gameState = GAME;
                levelController->nextLevel();
                setNextLevel = false;
                continue;
            }

            glm::vec3 loadPos = stateController->getGameObject(tpPlaygroundIndex + 2)->getPosition();
            float loadDistance = distance(playerPos, loadPos);
            if(loadDistance < 1) {
                gameState = GAME;
                levelController->loadSaved();
                setNextLevel = false;
                continue;
            }

        } else if(gameState == GAME) {
            glm::vec3 objectivePos = stateController->getObjective()->getPosition();
            float playerDistance = distance(playerPos, objectivePos);
            if(playerDistance < 1) {
                setNextLevel = true;
            }

            if(setNextLevel) {
                bool gameFinished = levelController->nextLevel();
                if(gameFinished) {
                    gameState = START;
                    tpPlaygroundIndex = levelController->home();
                }
                setNextLevel = false;
                lastFrame = glfwGetTime();
            }
        }

        if(gameState != START && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            gameState = START;
            tpPlaygroundIndex = levelController->home();
        }

        if(gameState == GAME && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            writeSaveFile(stateController->getPlayer()->getPosition(), levelController->getCurrentLevel());
            std::cout << " ** Game Saved **" << '\n';
        }

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
        stateController->update(deltaTime);
        stateController->render(deltaTime);

		// Update other events like input handling
		glfwPollEvents();

        // Use ImGui functions between here and Render()
        // ImGui::NewFrame();
        // // This creates a window
        // ImGui::Begin("Window Title Here");
        // ImGui::Text("Hello, world!");
        // ImGui::End();
        // // ImGui functions end here
        // ImGui::Render();


		// Put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

    if(gameState == GAME) {
        writeSaveFile(stateController->getPlayer()->getPosition(), levelController->getCurrentLevel());
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


void setupGUI(GLFWwindow* window) {
    unsigned char* pixels;
    int width,
        height,
        display_width,
        display_height;
    GLuint g_FontTexture;

    ImGuiIO& io { ImGui::GetIO() };

    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

    // Get display size
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &display_width, &display_height);

    io.DisplaySize = ImVec2((float)width, (float)height);
    io.RenderDrawListsFn = rendererFn;
    io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

void rendererFn(ImDrawData* draw_data) {
    ImGuiIO& io { ImGui::GetIO() };
    int fb_width { (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x) };
    int fb_height { (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y) };

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}
