//
// Created by joaop on 10/26/2025.
//

#ifndef RACE_PRACTICE_GIWINDOW_H
#define RACE_PRACTICE_GIWINDOW_H
#pragma once
#include <functional>

class GlWindow {
public:
    void init(int argc, char** argv, int w, int h, const char* title);
    void runLoop();

    static void setDisplayFunc(std::function<void()> f);
    static void setIdleFunc(std::function<void()> f);
    static void setReshapeFunc(std::function<void(int,int)> f);
    static void setKeyboardDownFunc(std::function<void(unsigned char,int,int)> f);
    static void setKeyboardUpFunc(std::function<void(unsigned char,int,int)> f);
    static void setSpecialDownFunc(std::function<void(int,int,int)> f);
    static void setSpecialUpFunc(std::function<void(int,int,int)> f);
};

#endif //RACE_PRACTICE_GIWINDOW_H