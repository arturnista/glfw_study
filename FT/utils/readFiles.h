#ifndef __READ_FILES_HEADER__
#define __READ_FILES_HEADER__ 1

// #pragma warning(disable:4996)
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

#include "../vendor/json.h"

using tJson = nlohmann::json;
using namespace std;

const char* readFile(string filename);

tJson readSaveFile();

void writeSaveFile(glm::vec3 pos, int level);

tJson readConfigFile();

void readObjFile(string filename,
					vector<GLfloat>& pointsVector,
					vector<GLuint>& indexVector,
					vector<GLfloat>& normalVector,
					vector<GLfloat>& textureVector);

#endif
