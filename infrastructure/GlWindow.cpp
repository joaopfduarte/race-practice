//
// Created by joaop on 10/26/2025.
//

#include "GlWindow.h"

#include <cstdio>
#include <GL/glut.h>

static std::function<void()> g_display, g_idle;
static std::function<void(int,int)> g_reshape;
static std::function<void(unsigned char,int,int)> g_keyDown, g_keyUp;
static std::function<void(int,int,int)> g_spDown, g_spUp;

static void sDisplay(){ if(g_display) g_display(); }

static void sIdle(){ if(g_idle) g_idle(); }

static void sReshape(int w,int h){ if(g_reshape) g_reshape(w,h); }

static void sKeyDown(unsigned char k,int x,int y){ if(g_keyDown) g_keyDown(k,x,y); }

static void sKeyUp(unsigned char k,int x,int y){ if(g_keyUp) g_keyUp(k,x,y); }

static void sSpDown(int k,int x,int y){ if(g_spDown) g_spDown(k,x,y); }

static void sSpUp(int k,int x,int y){ if(g_spUp) g_spUp(k,x,y); }

void GlWindow::init(int argc, char** argv, int w, int h, const char* title){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(w,h);
    glutCreateWindow(title);

    glutDisplayFunc(sDisplay);
    glutIdleFunc(sIdle);
    glutReshapeFunc(sReshape);
    glutKeyboardFunc(sKeyDown);
    glutKeyboardUpFunc(sKeyUp);
    glutSpecialFunc(sSpDown);
    glutSpecialUpFunc(sSpUp);

    glClearColor(0.1f,0.1f,0.12f,1.0f);

    glEnable(GL_TEXTURE_2D);

    // Verificar se o contexto está OK
    const GLubyte* version = glGetString(GL_VERSION);
    std::fprintf(stderr, "[GlWindow] OpenGL Version: %s\n", version);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::fprintf(stderr, "[GlWindow] OpenGL error after init: 0x%x\n", err);
    }
}

void GlWindow::runLoop(){ glutMainLoop(); }

void GlWindow::setDisplayFunc(std::function<void()> f){ g_display=f; }

void GlWindow::setIdleFunc(std::function<void()> f){ g_idle=f; }

void GlWindow::setReshapeFunc(std::function<void(int,int)> f){ g_reshape=f; }

void GlWindow::setKeyboardDownFunc(std::function<void(unsigned char,int,int)> f){ g_keyDown=f; }

void GlWindow::setKeyboardUpFunc(std::function<void(unsigned char,int,int)> f){ g_keyUp=f; }

void GlWindow::setSpecialDownFunc(std::function<void(int,int,int)> f){ g_spDown=f; }

void GlWindow::setSpecialUpFunc(std::function<void(int,int,int)> f){ g_spUp=f; }