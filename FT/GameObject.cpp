#include "GameObject.h"

GameObject::GameObject (TexturesManager* tm) {
	this->vertexCounter = 0;
	this->VAO = 0;
	this->rotation = vec3(0.0f);
	this->model = mat4(0.0f);

	this->setSize(vec3(0.0f));
	this->setPosition(vec3(0.0f));

	this->shader = NULL;
	this->texturesManager = tm;
}

GameObject::GameObject (TexturesManager* tm, string fn, float size, vec3 color) : GameObject::GameObject (tm, fn, vec3(size), color) {}

GameObject::GameObject (TexturesManager* tm, string fn, vec3 size, vec3 color) {
	string filename = "./assets/objects/" + fn;

	std::vector<GLfloat> pointsVector = {};
	std::vector<GLuint> indexVector = {};
	std::vector<GLfloat> normalVector = {};
	std::vector<GLfloat> textureVector = {};

	fetchFileData(filename, pointsVector, indexVector, normalVector, textureVector);

	this->hasTexture = textureVector.size() > 0;

	int textureCounter = (pointsVector.size() / 3) * 2;
	int pointsCounter = pointsVector.size() * 2 + textureCounter;
	int vertexCounter = indexVector.size();

	cout << filename << "\t";
	cout << "vertex count" << " = " << pointsVector.size() << '\t';
	cout << "face count" << " = " << vertexCounter << '\n';

	GLfloat *points = new GLfloat[pointsCounter];

	float lineSize = 8;

	int counter = 0;
	int texCounter = 0;
	for (size_t i = 0; i < pointsCounter; i += lineSize) {
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

		if(this->hasTexture) {
			float mult1 = abs(points[i + 4] * size.y) + abs(points[i + 3] * size.x);
			float mult2 = abs(points[i + 5] * size.z) + abs(points[i + 3] * size.x);

			points[i + 6] = textureVector.at(texCounter) * 1;
			points[i + 7] = textureVector.at(texCounter + 1) * 1;
			if(filename.compare("./assets/objects/cube_normal.obj") == 0) {
				std::cout << "vt " << mult1 << "\t" << mult2 << '\n';
			}
		} else {
			points[i + 6] = 0.0f;
			points[i + 7] = 0.0f;
		}

		texCounter += 2;
		counter += 3;
	}

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointsCounter * sizeof(GLfloat), points, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	if(vertexCounter > 0) {
		GLuint *indexArray = new GLuint[vertexCounter];
		for (size_t i = 0; i < vertexCounter; i++) indexArray[i] = indexVector.at(i) - 1;

		GLuint EBO = 0;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertexCounter, indexArray, GL_STATIC_DRAW);
	}

    this->vertexCounter = vertexCounter;
    this->VAO = VAO;
    this->rotation = vec3(0.0f);
    this->model = rotate(this->model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));

	this->setSize(size);
	this->setPosition(vec3(0.0f));

	this->color = color;

	this->shader = new Shader("texture");
	this->texturesManager = tm;
}

void GameObject::update(GLFWwindow* window, float deltaTime) {}

GLuint GameObject::getVAO() {
    return this->VAO;
}

int GameObject::getVertexCounter() {
    return this->vertexCounter;
}

void GameObject::setTextureName(string name) {
	this->textureName = name;
}

vec3 GameObject::getSize() {
    return this->size;
}

vec3 GameObject::getPosition() {
    return this->position;
}

float GameObject::getPositionX() {
    return this->position.x;
}

float GameObject::getPositionY() {
    return this->position.y;
}

float GameObject::getPositionZ() {
    return this->position.z;
}

void GameObject::setPosition(vec3 position) {
    this->position = position;
}

void GameObject::setPositionX(float value) {
    this->position.x = value;
}

void GameObject::setPositionY(float value) {
    this->position.y = value;
}

void GameObject::setPositionZ(float value) {
    this->position.z = value;
}

void GameObject::setSize(float size) {
    this->size = vec3(size);
}

void GameObject::setSize(vec3 size) {
    this->size = size;
}

void GameObject::rotateX(float value) {
    this->rotation.x += value;
}

void GameObject::rotateY(float value) {
    this->rotation.y += value;
}

void GameObject::rotateZ(float value) {
    this->rotation.z += value;
}

void GameObject::render(Camera* camera, tLight light) {
	if(this->shader == NULL) {
		return;
	}

	glUseProgram(this->shader->getProgram());

	// Apply the model, view and projection on the shader created
	mat4 model;
	model = translate(model, this->position);
	model = glm::rotate(model, this->rotation.x, vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, this->rotation.y, vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, this->rotation.z, vec3(0.0f, 0.0f, 1.0f));
	model = scale(model, this->size);

	mat4 view = camera->getView();
	mat4 projection = camera->getProjection();

	this->shader->use("objectColor", this->color);

	// Apply camera position
	this->shader->use("viewPos", camera->getPosition());

	this->shader->use("model", model);
	this->shader->use("inverseModel", inverse(model));
	this->shader->use("view", view);
	this->shader->use("projection", projection);

	// Apply lighting
	this->shader->use("lightColor", light.color);
	this->shader->use("lightPosition", light.position);

	if(this->hasTexture) {
		glBindTexture(GL_TEXTURE_2D, this->texturesManager->getTexture(this->textureName));
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCounter, GL_UNSIGNED_INT, 0);
}
