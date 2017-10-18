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

using namespace std;

const char* readFile(string filename);

void readObjFile(string filename,
					vector<GLfloat>& pointsVector,
					vector<GLuint>& indexVector,
					vector<GLfloat>& normalVector,
					vector<GLfloat>& textureVector);

#endif
