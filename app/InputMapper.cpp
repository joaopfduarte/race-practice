//
// Created by joaop on 10/26/2025.
//

#include "InputMapper.h"
#include <GL/glut.h>

static bool isAccel(int key) {return key == 'w' || key == 'W';}
static bool isBrake(int key) {return key == 's' || key == 'S';}
static bool isLeft(int key) {return key == 'a' || key == 'A';}
static bool isRight(int key) {return key == 'd' || key == 'D';}

void InputMapper::onKeyDown(unsigned char key){ down_.insert((int)key); }
void InputMapper::onKeyUp(unsigned char key){ down_.erase((int)key); }

void InputMapper::onSpecialDown(int key){ down_.insert(1000+key); }
void InputMapper::onSpecialUp(int key){ down_.erase(1000+key); }

CarInputState InputMapper::current() const {
    CarInputState s;
    // WASD (controle)
    for (int k: down_) {
        if (k<1000) {
            if (isAccel(k)) s.accel=true;
            if (isBrake(k)) s.brake=true;
            if (isLeft(k))  s.left =true;
            if (isRight(k)) s.right=true;
        }
    }
    // setas (controle)
    auto has = [&](int code){ return down_.count(1000+code)>0; };
    if (has(GLUT_KEY_UP))    s.accel = true  || s.accel;
    if (has(GLUT_KEY_DOWN))  s.brake = true  || s.brake;
    if (has(GLUT_KEY_LEFT))  s.left  = true  || s.left;
    if (has(GLUT_KEY_RIGHT)) s.right = true  || s.right;
    return s;
}