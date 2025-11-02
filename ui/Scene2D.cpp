#include "Scene2D.h"
#include "../infrastructure/GlRenderer2D.h"
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>

Scene2D::Scene2D() : pistaTex_() {
}

bool Scene2D::init() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    obstacles_.clear();
    spawnCooldown_ = 0.5f;
    flashTimeRemaining_ = 0.0f;
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

    // Reinicia novos estados
    obstacles_.clear();
    spawnCooldown_ = 0.5f;
    flashTimeRemaining_ = 0.0f;
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

    // Atualiza flash de colisão
    if (flashTimeRemaining_ > 0.0f) {
        flashTimeRemaining_ = std::max(0.0f, flashTimeRemaining_ - static_cast<float>(dt));
    }

    // Spawning de obstáculos
    spawnCooldown_ -= static_cast<float>(dt);
    if (spawnCooldown_ <= 0.0f) {
        // parâmetros
        float edgeMargin = 18.0f;
        float centerX = trackX_ + trackW_ * 0.5f;
        float centerY = trackY_ + trackH_ * 0.5f;
        float speedPx = 160.0f;
        float radius = 12.0f + static_cast<float>(std::rand() % 9);

        // escolhe lado: 0=esquerda,1=direita,2=topo
        int side = std::rand() % 3;
        Obstacle obstacle{};
        if (side == 0) {
            obstacle.x = trackX_ - edgeMargin;
            obstacle.y = trackY_ + static_cast<float>(std::rand()) / RAND_MAX * trackH_;
        } else if (side == 1) {
            obstacle.x = trackX_ + trackW_ + edgeMargin;
            obstacle.y = trackY_ + static_cast<float>(std::rand()) / RAND_MAX * trackH_;
        } else {
            obstacle.x = trackX_ + static_cast<float>(std::rand()) / RAND_MAX * trackW_;
            obstacle.y = trackY_ + trackH_ + edgeMargin;
        }
        // direção para o centro
        float dx = centerX - obstacle.x;
        float dy = centerY - obstacle.y;
        float len = std::sqrt(dx*dx + dy*dy);
        if (len < 1.0f) { dx = 1.0f; dy = 0.0f; len = 1.0f; }
        obstacle.vx = dx / len * speedPx;
        obstacle.vy = dy / len * speedPx;
        obstacle.radius = radius;
        obstacle.ttl = 10.0f;
        obstacles_.push_back(obstacle);

        // próximo spawn em 0.6..1.8s
        float r01 = static_cast<float>(std::rand()) / RAND_MAX;
        spawnCooldown_ = 0.6f + r01 * 1.2f;
    }

    // Atualiza obstáculos e detecta colisão via amostragem da borda a cada 15 graus
    const float carCollisionRadius = 14.0f;
    for (auto &o : obstacles_) {
        o.x += o.vx * static_cast<float>(dt);
        o.y += o.vy * static_cast<float>(dt);
        o.ttl -= static_cast<float>(dt);

        bool collided = false;
        // 24 amostras (360 / 15)
        for (int i = 0; i < 24; ++i) {
            float t = (static_cast<float>(i) * 15.0f) * 3.1415926535f / 180.0f;
            float px = o.x + std::cos(t) * o.radius;
            float py = o.y + std::sin(t) * o.radius;
            float dx = px - car_.x;
            float dy = py - car_.y;
            if (dx*dx + dy*dy <= carCollisionRadius * carCollisionRadius) {
                collided = true;
                break;
            }
        }
        if (collided) {
            failures_++;
            // Encerra se bater 5x
            if (failures_ == 5) {
                exit(0);
            }
            flashTimeRemaining_ = 0.25f;
            // marca TTL para remover já
            o.ttl = 0.0f;
        }
    }

    // remove obstáculos fora da pista
    const float removeMargin = 24.0f;
    obstacles_.erase(
        std::remove_if(obstacles_.begin(), obstacles_.end(), [&](const Obstacle& o){
            bool out = (o.x < trackX_ - removeMargin) || (o.x > trackX_ + trackW_ + removeMargin) ||
                       (o.y < trackY_ - removeMargin) || (o.y > trackY_ + trackH_ + removeMargin) ||
                       (o.ttl <= 0.0f);
            return out;
        }),
        obstacles_.end()
    );
}

void Scene2D::draw(GlRenderer2D &r) {
    r.drawFilledRect(trackX_, trackY_, trackW_, trackH_, 0.12f, 0.12f, 0.12f);

    r.drawLine(trackX_, trackY_, trackX_ + trackW_, trackY_, 1, 1, 1, 2);
    r.drawLine(trackX_, trackY_ + trackH_, trackX_ + trackW_, trackY_ + trackH_, 1, 1, 1, 2);
    r.drawLine(trackX_, trackY_, trackX_, trackY_ + trackH_, 1, 1, 1, 2);
    r.drawLine(trackX_ + trackW_, trackY_, trackX_ + trackW_, trackY_ + trackH_, 1, 1, 1, 2);

    {
        float centerX = startLineX1_;
        float dashLen = 28.0f;   // comprimento do traço
        float gapLen  = 18.0f;   // espaçamento entre traços
        float period  = dashLen + gapLen;
        float thickness = 6.0f;

        // fator de deslocamento
        float scrollFactor = -0.6f;
        float phase = std::fmod(-car_.y * scrollFactor, period);
        if (phase < 0.0f) phase += period;

        float yStart = trackY_ - phase;
        float yLimit = trackY_ + trackH_;
        for (float y = yStart; y < yLimit; y += period) {
            float segStart = std::max(y, trackY_);
            float segEnd   = std::min(y + dashLen, yLimit);
            if (segEnd > segStart) {
                r.drawLine(centerX, segStart, centerX, segEnd, 1.0f, 1.0f, 0.0f, thickness);
            }
        }
    }

    // 3.5) Desenha obstáculos
    for (const auto& o : obstacles_) {
        // cor laranja para visibilidade
        r.drawCircle(o.x, o.y, o.radius, 1.0f, 0.6f, 0.0f, 24);
    }
 
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

    // Cor do carro: vermelho durante flash de colisão, verde caso contrário
    float carR = (flashTimeRemaining_ > 0.0f) ? 1.0f : 0.0f;
    float carG = (flashTimeRemaining_ > 0.0f) ? 0.0f : 1.0f;
    float carB = 0.0f;

    r.drawLine(v1.first, v1.second, v2.first, v2.second, carR, carG, carB, 2);
    r.drawLine(v2.first, v2.second, v3.first, v3.second, carR, carG, carB, 2);
    r.drawLine(v3.first, v3.second, v1.first, v1.second, carR, carG, carB, 2);
}
