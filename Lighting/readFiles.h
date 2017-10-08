#ifndef __READ_FILES_HEADER__
#define __READ_FILES_HEADER__ 1

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

using namespace std;
using namespace glm;

const char* readFile(string filename);

gameObject readObjectFile(string filename, float size, vec3 offset);

gameObject readObjectFile(string filename, float size = 1);

#endif
