//
// Created by joaop on 10/26/2025.
//

#include "Hud.h"
#include "../infrastructure/GlRenderer2D.h"
#include "Scene2D.h"
#include <cstdio>

void Hud::draw(GlRenderer2D& r, const Scene2D& s){
    char buf[256];
    std::snprintf(buf, sizeof(buf), "Voltas: %d/%d", s.laps(), s.lapTarget());
    r.drawText(20, r.viewportHeight()-28, buf, 1,1,0);

    r.drawText(20, r.viewportHeight()-46, buf, 1,1,1);

    std::snprintf(buf, sizeof(buf), "Velocidade: %.1f", s.speed());
    r.drawText(20, r.viewportHeight()-64, buf, 0.5f,1,1);

    std::snprintf(buf, sizeof(buf), "Falhas: %d", s.failures());
    r.drawText(20, r.viewportHeight()-82, buf, 1,0.5f,0.5f);
}
