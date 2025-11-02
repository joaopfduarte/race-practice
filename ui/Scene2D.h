#pragma once
#include "../app/InputMapper.h"
#include "../infrastructure/Texture.h"

class GlRenderer2D;

struct CarState {
    float x = 512.0f;
    float y = 120.0f;
    float angle = 3.1415926f / 2.0f;
    float speed = 0.0f;
};

class Scene2D {
public:
    Scene2D();

    bool init();

    void update(double dt, const CarInputState& in);

    void draw(GlRenderer2D& r);

    void reset();

    double currentLapTime() const { return currentLapTime_; }

    float speed() const { return car_.speed; }

    int failures() const { return failures_; }

private:
    // Geometria básica da pista (retangular para colisão simples)
    float trackX_ = 250.0f;
    float trackY_ = 55.0f;
    float trackW_ = 524.0f;
    float trackH_ = 658.0f;

    // Linha de largada (segmento vertical)
    float startLineX1_ = 512.0f, startLineY1_ = 80.0f;
    float startLineX2_ = 512.0f, startLineY2_ = 160.0f;

    // Direção de cruzamento para validar volta (do lado "de baixo" para "cima")
    bool lastStartSidePositive_ = false;

    // Textura da pista
    Texture pistaTex_;
    // Caminho padrão (assumindo execução a partir da raiz do projeto)
    std::string pistaPath_ = "assets/textures/pista.png";
    bool textureLoaded_ = false;

    // Estado do carro e métricas
    CarState car_;
    int laps_ = 0;
    int lapTarget_ = 3;
    int failures_ = 0; // reservado para colisão com buracos (futuro)
    double currentLapTime_ = 0.0;
    double bestLapTime_ = 0.0;
};