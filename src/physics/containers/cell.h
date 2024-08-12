#pragma once
#include <memory>
#include "../particle.h"

enum class CellType {
    EMPTY,
    PARTICLE,
    PLAYER
};

class Cell {
public:
    std::shared_ptr<Particle> particle;
    CellType type;

    Cell() {
        unsetParticle();
    }

    void setParticle(std::shared_ptr<Particle> ptcl) {
        type = CellType::PARTICLE;
        particle = ptcl;
    }
    void unsetParticle() {
        type = CellType::EMPTY;
        particle = nullptr;
    }
};