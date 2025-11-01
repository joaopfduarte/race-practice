#include "GlRenderer2D.h"
#include <GL/glut.h>
#include <cmath>

void GlRenderer2D::init(int w, int h) {
    resize(w, h);
    // Configuração padrão de clear color
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
}

void GlRenderer2D::resize(int w, int h) {
    vpW_ = w;
    vpH_ = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Projeção ortográfica igual ao tamanho da janela (0..w, 0..h)
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GlRenderer2D::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
}

void GlRenderer2D::endFrame() {
    glutSwapBuffers();
}

void GlRenderer2D::drawFilledRect(float x, float y, float w, float h, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
        glVertex2f(x,   y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y+h);
        glVertex2f(x,   y+h);
    glEnd();
}

void GlRenderer2D::drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float thickness) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(r,g,b);
    glLineWidth(thickness);
    glBegin(GL_LINES);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
    glEnd();
    glLineWidth(1.0f);
}

void GlRenderer2D::drawCircle(float cx, float cy, float radius, float r, float g, float b, int segments) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(r,g,b);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float t = (float)i / (float)segments * 2.0f * 3.1415926535f;
        float px = cx + std::cos(t) * radius;
        float py = cy + std::sin(t) * radius;
        glVertex2f(px, py);
    }
    glEnd();
}

void GlRenderer2D::drawText(float x, float y, const char* txt, float r, float g, float b, float /*scale*/) {
    // Nota: glutBitmap* não suporta escala real (apenas tamanhos fixos).
    // Para fontes escaláveis, seria necessário outra abordagem (FTGL, etc.).
    glDisable(GL_TEXTURE_2D);
    glColor3f(r,g,b);
    glRasterPos2f(x, y);
    for (const char* p = txt; *p; ++p) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
    }
}

void GlRenderer2D::drawTexturedQuad(float x, float y, float w, float h, GLuint texId) {
    if (texId == 0) {
        // fallback: se textura inválida, desenha retângulo branco
        drawFilledRect(x, y, w, h, 1,1,1);
        return;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f); glVertex2f(x,   y);
        glTexCoord2f(1.f, 0.f); glVertex2f(x+w, y);
        glTexCoord2f(1.f, 1.f); glVertex2f(x+w, y+h);
        glTexCoord2f(0.f, 1.f); glVertex2f(x,   y+h);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}