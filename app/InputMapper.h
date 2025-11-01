//
// Created by joaop on 10/26/2025.
//

#ifndef RACE_PRACTICE_INPUTMAPPER_H
#define RACE_PRACTICE_INPUTMAPPER_H
#pragma once
#include <unordered_set>

struct CarInputState {
    bool accel = false;
    bool brake = false;
    bool left = false;
    bool right = false;
};

class InputMapper {
public:
    void onKeyDown(unsigned char k);
    void onKeyUp(unsigned char k);
    void onSpecialDown(int k);
    void onSpecialUp(int k);
    CarInputState current() const;
private:
    std::unordered_set<int> down_;
};

#endif //RACE_PRACTICE_INPUTMAPPER_H