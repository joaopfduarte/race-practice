//
// Created by joaop on 10/26/2025.
//

#include "Time.h"
#include <GL/glut.h>

double Time::nowMs() {
    return (double)glutGet(GLUT_ELAPSED_TIME);
}