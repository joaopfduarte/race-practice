#include "Scene2D.h"
#include "../infrastructure/GlRenderer2D.h"
#include <cmath>
#include <algorithm>
#include <cstdio>

Scene2D::Scene2D() : pistaTex_() {
}

bool Scene2D::init() {
    return true;
}

void Scene2D::reset() {
    car_ = {};
    car_.x = 512.0f;
    car_.y = 120.0f;
    car_.angle = 3.1415926f / 2.0f; // para cima
    car_.speed = 0.0f;

    currentLapTime_ = 0.0;
    failures_ = 0;
}

void Scene2D::update(double dt, const CarInputState &in) {
    // Física simples
    const float accel = 500.0f;
    const float brake = 600.0f;
    const float drag = 0.98f;
    const float maxSpeed = 600.0f;
    const float steer = 2.5f; // rad/s

    if (in.left) car_.angle += steer * dt;
    if (in.right) car_.angle -= steer * dt;

    float forward = 0.0f;
    if (in.accel) forward += accel;
    if (in.brake) forward -= brake;

    car_.speed += forward * dt;
    car_.speed = std::clamp(car_.speed, -maxSpeed * 0.5f, maxSpeed);
    car_.speed *= drag;

    car_.x += std::cos(car_.angle) * car_.speed * dt;
    car_.y += std::sin(car_.angle) * car_.speed * dt;

    // Limites retangulares da pista
    if (car_.x < trackX_ || car_.x > trackX_ + trackW_ || car_.y < trackY_ || car_.y > trackY_ + trackH_) {
        // Reposiciona e zera velocidade (correção conservadora)
        car_.x = std::clamp(car_.x, trackX_ + 10.0f, trackX_ + trackW_ - 10.0f);
        car_.y = std::clamp(car_.y, trackY_ + 10.0f, trackY_ + trackH_ - 10.0f);
        car_.speed = 0.0f;
    }

    bool nearStartX = std::abs(car_.x - startLineX1_) < 10.0f;
    if (nearStartX) {
        bool nowPositive = car_.y > (startLineY2_);
        if (!lastStartSidePositive_ && nowPositive && car_.speed > 50.0f) {
            // Completou uma volta válida
            laps_++;
            if (bestLapTime_ <= 0.0 || currentLapTime_ < bestLapTime_) {
                bestLapTime_ = currentLapTime_;
            }
            currentLapTime_ = 0.0;
        }
        lastStartSidePositive_ = nowPositive;
    }

    currentLapTime_ += dt;

    // TODO (futuro): colisão com buracos e failures_++
}

void Scene2D::draw(GlRenderer2D &r) {
    // Carrega textura apenas no primeiro frame

    // 1) Pista como textura de fundo dentro da caixa (trackX/Y/W/H)
    r.drawFilledRect(trackX_, trackY_, trackW_, trackH_, 0.12f, 0.12f, 0.12f);

    // 2) Bordas da pista (opcional para reforço visual)
    r.drawLine(trackX_, trackY_, trackX_ + trackW_, trackY_, 1, 1, 1, 2);
    r.drawLine(trackX_, trackY_ + trackH_, trackX_ + trackW_, trackY_ + trackH_, 1, 1, 1, 2);
    r.drawLine(trackX_, trackY_, trackX_, trackY_ + trackH_, 1, 1, 1, 2);
    r.drawLine(trackX_ + trackW_, trackY_, trackX_ + trackW_, trackY_ + trackH_, 1, 1, 1, 2);

    // 3) Linha de largada/chegada (sobre a textura)
    r.drawLine(startLineX1_, startLineY1_, startLineX2_, startLineY2_, 1, 0, 0, 3);

    // 4) Carro como triângulo orientado pelo ângulo
    float bx = car_.x, by = car_.y;
    float ang = car_.angle;
    float L = 18.0f, W = 12.0f;

    // Vértices no espaço local (triângulo: ponta à frente)
    float px1 = L, py1 = 0;
    float px2 = -L, py2 = W;
    float px3 = -L, py3 = -W;

    auto rot = [&](float px, float py) {
        float rx = bx + std::cos(ang) * px - std::sin(ang) * py;
        float ry = by + std::sin(ang) * px + std::cos(ang) * py;
        return std::pair<float, float>{rx, ry};
    };

    auto v1 = rot(px1, py1);
    auto v2 = rot(px2, py2);
    auto v3 = rot(px3, py3);

    r.drawLine(v1.first, v1.second, v2.first, v2.second, 0, 1, 0, 2);
    r.drawLine(v2.first, v2.second, v3.first, v3.second, 0, 1, 0, 2);
    r.drawLine(v3.first, v3.second, v1.first, v1.second, 0, 1, 0, 2);
}
