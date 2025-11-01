#pragma once
#include <GL/gl.h>

class GlRenderer2D {
public:
    void init(int w, int h);

    void resize(int w, int h);

    void beginFrame();
    void endFrame();

    void drawFilledRect(float x, float y, float w, float h, float r, float g, float b);
    void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float thickness = 1.0f);
    void drawCircle(float cx, float cy, float radius, float r, float g, float b, int segments = 32);
    void drawText(float x, float y, const char* txt, float r, float g, float b, float scale = 1.0f);

    void drawTexturedQuad(float x, float y, float w, float h, GLuint texId);

    int viewportWidth() const { return vpW_; }
    int viewportHeight() const { return vpH_; }

private:
    int vpW_ = 0;
    int vpH_ = 0;
};