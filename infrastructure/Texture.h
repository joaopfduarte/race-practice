//
// Created by joaop on 10/26/2025.
//

#ifndef RACE_PRACTICE_TEXTURE_H
#define RACE_PRACTICE_TEXTURE_H

#pragma  once
#include <string>
#include <GL/gl.h>

class Texture {
public:
    Texture() = default;
    ~Texture();

    bool loadFromFile(const std::string& path, bool flipVertically = true);

    void bind() const;
    void unbind() const;

    int width() const { return width_; }
    int height() const { return height_; }
    GLuint id() const { return texId_; }

private:
    void release();
    GLuint texId_ = 0;
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
};

#endif //RACE_PRACTICE_TEXTURE_H