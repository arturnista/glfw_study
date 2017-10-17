#include "ResourcesManager.h"

ResourcesManager::ResourcesManager() {
    textureMap["grass"] = this->processTexture("./assets/textures/grass.jpg");
    textureMap["stone"] = this->processTexture("./assets/textures/stone.jpg");
    textureMap["container"] = this->processTexture("./assets/textures/container.jpg");
    textureMap["wall"] = this->processTexture("./assets/textures/wall.jpg");

    objectMap["grass_normal"] = this->processObjectFile("./assets/objects/grass_normal.obj");
    objectMap["dirt_normal"] = this->processObjectFile("./assets/objects/dirt_normal.obj");
    objectMap["cube_normal"] = this->processObjectFile("./assets/objects/cube_normal.obj");
    objectMap["bunny_normal"] = this->processObjectFile("./assets/objects/bunny_normal.obj");
    objectMap["cube"] = this->processObjectFile("./assets/objects/cube.obj");
}

tObject ResourcesManager::processObjectFile(std::string filename) {
	std::vector<GLfloat> pointsVector = {};
	std::vector<GLuint> indexVector = {};
	std::vector<GLfloat> normalVector = {};
	std::vector<GLfloat> textureVector = {};

	fetchFileData(filename, pointsVector, indexVector, normalVector, textureVector);

	bool hasTexture = textureVector.size() > 0;

	int textureCounter = (pointsVector.size() / 3) * 2;
	int pointsCounter = pointsVector.size() * 2 + textureCounter;
	int verticesCounter = indexVector.size();

	GLfloat *points = new GLfloat[pointsCounter];

	int itemsPerPoint = 8;

	int counter = 0;
	int texCounter = 0;
	for (size_t i = 0; i < pointsCounter; i += itemsPerPoint) {
		// Position
		points[i + 0] = pointsVector.at(counter);
		points[i + 1] = pointsVector.at(counter + 1);
		points[i + 2] = pointsVector.at(counter + 2);

		if(normalVector.size() > 0) {
			points[i + 3] = normalVector.at(counter);
			points[i + 4] = normalVector.at(counter + 1);
			points[i + 5] = normalVector.at(counter + 2);
		} else {
			points[i + 3] = 1.0f;
			points[i + 4] = 0.0f;
			points[i + 5] = 0.0f;
		}

		if(hasTexture) {
			points[i + 6] = textureVector.at(texCounter);
			points[i + 7] = textureVector.at(texCounter + 1);
		} else {
			points[i + 6] = 0.0f;
			points[i + 7] = 0.0f;
		}

		texCounter += 2;
		counter += 3;
	}

	GLuint *vertices = new GLuint[verticesCounter];
	for (size_t i = 0; i < verticesCounter; i++) vertices[i] = indexVector.at(i) - 1;


    cout << "Object: " << filename << "\t";
    cout << "vertex count" << " = " << pointsVector.size() << '\t';
    cout << "face count" << " = " << verticesCounter << '\n';
    return {
    	pointsVector,
    	indexVector,
    	normalVector,
    	textureVector,
    	hasTexture,
        itemsPerPoint,
    	points,
    	pointsCounter,
    	vertices,
    	verticesCounter
    };
}

unsigned int ResourcesManager::processTexture(std::string filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *textureData = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_default);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture " << filename << std::endl;
    }
    stbi_image_free(textureData);


    cout << "Texture: " << filename << "\t";
    cout << "Size (" << width << ", " << height << ")" << '\n';
    return texture;
}

tObject ResourcesManager::getObject(std::string objectName) {
    return this->objectMap[objectName];
}

unsigned int ResourcesManager::getTexture(std::string textureName) {
    return this->textureMap[textureName];
}
