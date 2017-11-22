#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

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

void readObjFile(string filename,
					vector<GLfloat>& pointsVector,
					vector<GLuint>& indexVector,
					vector<GLfloat>& normalVector,
					vector<GLfloat>& textureVector) {
	ifstream openedFile(filename.c_str());
	if (openedFile.is_open()) {
		string line;
		string strNumber;

		while(!openedFile.eof()) {
			openedFile >> line;
			if(openedFile.eof()) break;

			if(line.compare("v") == 0) {
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				pointsVector.push_back( stof(strNumber) );
			} else if(line.compare("vn") == 0) {
				openedFile >> strNumber;
				normalVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				normalVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				normalVector.push_back( stof(strNumber) );
			} else if(line.compare("vt") == 0) {
				openedFile >> strNumber;
				textureVector.push_back( stof(strNumber) );
				openedFile >> strNumber;
				textureVector.push_back( stof(strNumber) );
			} else if(line.compare("f") == 0) {
				int count = 3;
				for (size_t i = 0; i < count; i++) {
					openedFile >> strNumber;
					int doubleIdx = strNumber.find("//");
					if(doubleIdx > 0) {
						string fstNumber = strNumber.substr(0, doubleIdx);
						string secNumber = strNumber.substr(doubleIdx + 2);
						indexVector.push_back( stof(fstNumber) );
						// normalVector.push_back( stof(secNumber) );
					} else {
						int singleIdx = strNumber.find("/");
						if(singleIdx > 0) {
							string fstNumber = strNumber.substr(0, singleIdx);
							string nextString = strNumber.substr(singleIdx + 1);
							int nextIndex = nextString.find("/");
							string secNumber = nextString.substr(0, nextIndex);
							string trdNumber = nextString.substr(nextIndex + 1);

							indexVector.push_back( stof(fstNumber) );
							// normalVector.push_back( stof(trdNumber) );
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
