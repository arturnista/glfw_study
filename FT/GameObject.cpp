#include "GameObject.h"

GameObject::GameObject (ResourcesManager* rm) {
	this->vertexCounter = 0;
	this->VAO = 0;
	this->rotation = vec3(0.0f);
	this->model = mat4(0.0f);

	this->setSize(vec3(0.0f));
	this->setPosition(vec3(0.0f));

	this->shader = NULL;
	this->resourcesManager = rm;
}

GameObject::GameObject (ResourcesManager* rm, string fn, float size, vec3 color) : GameObject::GameObject (rm, fn, vec3(size), color) {}

GameObject::GameObject (ResourcesManager* rm, string fn, vec3 size, vec3 color) {
	tObject object = rm->getObject(fn);

	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, object.pointsCounter * sizeof(GLfloat), object.points, GL_STATIC_DRAW);

	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, object.itemsPerPoint * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, object.itemsPerPoint * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, object.itemsPerPoint * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	if(object.verticesCounter > 0) {
		GLuint EBO = 0;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * object.verticesCounter, object.vertices, GL_STATIC_DRAW);
	}

	this->hasTexture = object.hasTexture;
    this->vertexCounter = object.verticesCounter;
    this->VAO = VAO;
    this->rotation = vec3(0.0f);
    this->model = rotate(this->model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));

	this->setSize(size);
	this->setPosition(vec3(0.0f));

	this->color = color;

	this->shader = new Shader("texture");
	this->resourcesManager = rm;
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
	model = rotate(model, this->rotation.x, vec3(1.0f, 0.0f, 0.0f));
    model = rotate(model, this->rotation.y, vec3(0.0f, 1.0f, 0.0f));
    model = rotate(model, this->rotation.z, vec3(0.0f, 0.0f, 1.0f));
	model = scale(model, this->size);

	this->shader->use("objectColor", this->color);

	// Apply camera position
	this->shader->use("viewPos", camera->getPosition());

	this->shader->use("model", model);
	this->shader->use("inverseModel", inverse(model));
	this->shader->use("view", camera->getView());
	this->shader->use("projection", camera->getProjection());

	// Apply lighting
	this->shader->use("lightColor", light.color);
	this->shader->use("lightPosition", light.position);

	if(this->hasTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->resourcesManager->getTexture(this->textureName));
		this->shader->use("outTexture", 0);
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->vertexCounter, GL_UNSIGNED_INT, 0);
}
