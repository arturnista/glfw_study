#include "GameObject.h"

GameObject::GameObject (string filename, float size, vec3 color) {
	std::vector<GLfloat> pointsVector = {};
	std::vector<GLuint> indexVector = {};
	std::vector<GLfloat> normalVector = {};

	fetchFileData(filename, pointsVector, indexVector, normalVector);


	int pointsCounter = pointsVector.size() * 3;
	int vertexCounter = indexVector.size();

	cout << filename << "\t";
	cout << "vertex count" << " = " << pointsVector.size() << '\t';
	cout << "face count" << " = " << vertexCounter << '\n';

	GLfloat *points = new GLfloat[pointsCounter];

	float lineSize = 9;

	int counter = 0;
	for (size_t i = 0; i < pointsCounter; i += lineSize) {
		// Position
		points[i + 0] = pointsVector.at(counter);
		points[i + 1] = pointsVector.at(counter + 1);
		points[i + 2] = pointsVector.at(counter + 2);

		// Color
		points[i + 3] = color.x;
		points[i + 4] = color.y;
		points[i + 5] = color.z;

		if(normalVector.size() > 0) {
			points[i + 6] = normalVector.at(counter);
			points[i + 7] = normalVector.at(counter + 1);
			points[i + 8] = normalVector.at(counter + 2);
		} else {
			points[i + 6] = 1.0f;
			points[i + 7] = 0.0f;
			points[i + 8] = 0.0f;
		}

		counter += 3;
	}
	// copy(pointsVector.begin(), pointsVector.end(), points);
	// if(size != 1) for (size_t i = 0; i < pointsCounter; i++) points[i] = points[i] * size;

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

	// Colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, lineSize * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	if(vertexCounter > 0) {
		GLuint *indexArray = new GLuint[vertexCounter];
		copy(indexVector.begin(), indexVector.end(), indexArray);
		for (size_t i = 0; i < vertexCounter; i++) indexArray[i] = indexArray[i] - 1;

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
}

void GameObject::update(GLFWwindow* window, float deltaTime) {}

GLuint GameObject::getVAO() {
    return this->VAO;
}

int GameObject::getVertexCounter() {
    return this->vertexCounter;
}

float GameObject::getSize() {
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
	std::cout << "X: " << value << '\n';
}

void GameObject::setPositionY(float value) {
    this->position.y = value;
	std::cout << "Y: " << value << '\n';
}

void GameObject::setPositionZ(float value) {
    this->position.z = value;
	std::cout << "Z: " << value << '\n';
}

void GameObject::setSize(float size) {
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

void GameObject::render(Shader* shader, mat4 view, mat4 projection) {
	// Apply the model, view and projection on the shader created
	mat4 model;
	model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	model = translate(model, this->position);
	model = glm::rotate(model, this->rotation.x, vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, this->rotation.y, vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, this->rotation.z, vec3(0.0f, 0.0f, 1.0f));
	model = scale(model, vec3(this->size));

	shader->use("model", model);
	shader->use("inverseModel", inverse(model));
	shader->use("view", view);
	shader->use("projection", projection);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCounter, GL_UNSIGNED_INT, 0);
}
