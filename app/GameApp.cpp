#include <memory>
#include "GameApp.h"
#include "Time.h"
#include <GL/glut.h>
#include <cstdio>

static GameApp *g_app = nullptr;

GameApp::GameApp() {
}

void GameApp::run(int argc, char **argv) {
    g_app = this;

    std::puts("[GameApp] init window");
    window_ = std::make_unique<GlWindow>();
    window_->init(argc, argv, width_, height_, "Corrida 2D - CG");

    std::puts("[GameApp] init renderer");
    renderer_ = std::make_unique<GlRenderer2D>();
    renderer_->init(width_, height_);

    scene_ = std::make_unique<Scene2D>();
    hud_ = std::make_unique<Hud>();
    input_ = std::make_unique<InputMapper>();

    GlWindow::setDisplayFunc([]() { g_app->onDisplay(); });
    GlWindow::setIdleFunc([]() { g_app->onIdle(); });
    GlWindow::setReshapeFunc([](int w, int h) { g_app->onReshape(w, h); });
    GlWindow::setKeyboardDownFunc([](unsigned char k, int x, int y) { g_app->onKeyboardDown(k, x, y); });
    GlWindow::setKeyboardUpFunc([](unsigned char k, int x, int y) { g_app->onKeyboardUp(k, x, y); });
    GlWindow::setSpecialDownFunc([](int k, int x, int y) { g_app->onSpecialDown(k, x, y); });
    GlWindow::setSpecialUpFunc([](int k, int x, int y) { g_app->onSpecialUp(k, x, y); });

    std::puts("[GameApp] init scene (load textures)");
    if (!scene_->init()) {
        std::puts("[GameApp] Warning: Scene initialization failed (texture not loaded)");
    }

    lastTimeMs_ = Time::nowMs();
    std::puts("[GameApp] entering main loop");
    window_->runLoop();
    std::puts("[GameApp] after main loop (should not reach here)");
}

void GameApp::onDisplay() {
    render();
}

void GameApp::onIdle() {
    double now = Time::nowMs();
    double dt = (now - lastTimeMs_) / 700.0;
    lastTimeMs_ = now;

    if (!paused_ && !quit_) {
        update(dt);
    }
    glutPostRedisplay();
}

void GameApp::onReshape(int w, int h) {
    width_ = w;
    height_ = h;
    renderer_->resize(w, h);
}

void GameApp::onKeyboardDown(unsigned char key, int, int) {
    if (key == 27) {
        quit_ = true;
        exit(0);
    }
    if (key == 'p' || key == 'P') paused_ = !paused_;
    if (key == 'r' || key == 'R') {
        scene_->reset();
    }
    input_->onKeyDown(key);
}

void GameApp::onKeyboardUp(unsigned char key, int, int) {
    input_->onKeyUp(key);
}

void GameApp::onSpecialDown(int key, int, int) {
    input_->onSpecialDown(key);
}

void GameApp::onSpecialUp(int key, int, int) {
    input_->onSpecialUp(key);
}

void GameApp::update(double dt) {
    scene_->update(dt, input_->current());
}

void GameApp::render() {
    renderer_->beginFrame();
    scene_->draw(*renderer_);
    hud_->draw(*renderer_, *scene_);
    renderer_->endFrame();
}
