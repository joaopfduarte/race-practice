#pragma once
#include <memory>

#include "../infrastructure/GlWindow.h"
#include "../infrastructure/GlRenderer2D.h"
#include "../ui/Scene2D.h"
#include "../ui/Hud.h"
#include "InputMapper.h"

class GlWindow;
class Hud;

class GameApp {
public:
    GameApp();
    void run(int argc, char** argv);

    void onDisplay();
    void onIdle();
    void onReshape(int w, int h);
    void onKeyboardDown(unsigned char key, int x, int y);
    void onKeyboardUp(unsigned char key, int x, int y);
    void onSpecialDown(int key, int x, int y);
    void onSpecialUp(int key, int x, int y);

private:
    void update(double dt);
    void render();

    bool paused_ = false;
    bool quit_ = false;

    int width_ = 1024;
    int height_ = 768;

    double lastTimeMs_ = 0.0;

    std::unique_ptr<GlWindow>      window_;
    std::unique_ptr<GlRenderer2D>  renderer_;
    std::unique_ptr<Scene2D>       scene_;
    std::unique_ptr<Hud>           hud_;
    std::unique_ptr<InputMapper>   input_;
};
