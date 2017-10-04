#include "readFiles.h"

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

void fetchFileData(string filename, vector<GLfloat>& pointsVector, vector<GLuint>& indexVector) {
	ifstream openedFile(filename.c_str());
	if (openedFile.is_open()) {
		string line;
		string strNumber;

		while(!openedFile.eof()) {
			openedFile >> line;

			if(line.compare("v") == 0) {
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
			} else if(line.compare("f") == 0) {

				int count = 3;
				for (size_t i = 0; i < count; i++) {
					openedFile >> strNumber;
					int doubleIdx = strNumber.find("//");
					if(doubleIdx > 0) {
						string fstNumber = strNumber.substr(0, doubleIdx);
						string secNumber = strNumber.substr(doubleIdx + 2);
						indexVector.push_back( stof(fstNumber) );
					} else {
						int singleIdx = strNumber.find("/");
						if(singleIdx > 0) {
							string fstNumber = strNumber.substr(0, singleIdx);
							string nextString = strNumber.substr(singleIdx + 1);
							int nextIndex = nextString.find("/");
							string secNumber = nextString.substr(0, nextIndex);
							string trdNumber = nextString.substr(nextIndex + 1);

							indexVector.push_back( stof(fstNumber) );
						} else {
							indexVector.push_back( stof(strNumber) );
						}
					}
				}

			}
		}
		openedFile.close();
	}
}

gameObject readObjectFile(string filename, float size, vec3 offset) {
	std::vector<GLfloat> pointsVector = {};
    std::vector<GLuint> indexVector = {};

	fetchFileData(filename, pointsVector, indexVector);

	int pointsCounter = pointsVector.size() * 3;
	int vertexCounter = indexVector.size();

	cout << filename << "\t";
	cout << "vertex count" << " = " << pointsVector.size() << '\t';
	cout << "face count" << " = " << vertexCounter << '\n';

	GLfloat *points = new GLfloat[pointsCounter];
	glm::vec3 color = glm::vec3(
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
	);
	int counter = 0;
	for (size_t i = 0; i < pointsCounter; i += 9) {
		// Position
		points[i + 0] = pointsVector.at(counter++);
		points[i + 1] = pointsVector.at(counter++);
		points[i + 2] = pointsVector.at(counter++);

		// Normal
		points[i + 3] = 1.0f;
		points[i + 4] = 0.0f;
		points[i + 5] = 0.0f;

		// Color
		points[i + 6] = color.x; // R
		points[i + 7] = color.y; // G
		points[i + 8] = color.z; // B
	}
	// copy(pointsVector.begin(), pointsVector.end(), points);
	// if(size != 1) for (size_t i = 0; i < pointsCounter; i++) points[i] = points[i] * size;

	GLuint *indexArray = new GLuint[vertexCounter];
	copy(indexVector.begin(), indexVector.end(), indexArray);
	for (size_t i = 0; i < vertexCounter; i++) indexArray[i] = indexArray[i] - 1;

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointsCounter * sizeof(GLfloat), points, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float lineSize = 9;

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Colors
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	GLuint EBO = 0;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertexCounter, indexArray, GL_STATIC_DRAW);

	return {
		VAO,
		vertexCounter,
		size,
	};
}

gameObject readObjectFile(string filename, float size) {
	return readObjectFile(filename, size, vec3(0.0f, 0.0f, 0.0f));
}
