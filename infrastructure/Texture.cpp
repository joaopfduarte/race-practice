#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"
#include <cstdio>

Texture::~Texture() { release(); }

void Texture::release() {
    if (texId_ != 0) {
        glDeleteTextures(1, &texId_);
        texId_ = 0;
    }
}

bool Texture::loadFromFile(const std::string& path, bool flipVertically) {
    release();
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &channels_, 0);
    if (!data) {
        std::fprintf(stderr, "Failed to load image: %s\n", path.c_str());
        return false;
    }

    GLenum format = GL_RGB;
    if (channels_ == 1) format = GL_RED;
    else if (channels_ == 3) format = GL_RGB;
    else if (channels_ == 4) format = GL_RGBA;

    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return true;
}

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, texId_); }
void Texture::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }