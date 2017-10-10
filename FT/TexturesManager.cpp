#include "TexturesManager.h"

TexturesManager::TexturesManager() {
    textureMap["container"] = this->processTexture("./assets/textures/container.jpg");
    textureMap["wall"] = this->processTexture("./assets/textures/wall.jpg");
}

unsigned int TexturesManager::processTexture(std::string filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *textureData = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (textureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture " << filename << std::endl;
    }
    stbi_image_free(textureData);

    return texture;
}

unsigned int TexturesManager::getTexture(std::string textureName) {
    return this->textureMap[textureName];
}
