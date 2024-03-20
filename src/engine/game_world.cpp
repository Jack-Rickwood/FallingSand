#include <memory>
#include "game_world.h"

void GameWorld::evolve(int frame_num) {
    updateParticles(frame_num);
}

void GameWorld::addParticle(std::shared_ptr<Particle> ptcl) {
    if (state.checkValidIndex(ptcl->position)) {
        state.getElement(ptcl->position).setParticle(ptcl);
        ptcl->acceleration = ptcl->getGravity();
    }
}

void GameWorld::updateParticles(int frame_num) {
    bool flip = frame_num % 2 == 0;
    if (flip) {
        for (int y = state.height - 1; y >= 0; y--) {
            for (int x = 0; x < state.width; x++) {
                Cell& curr = state.getElement(x, y);
                if (curr.type == CellType::PARTICLE) {
                    curr.particle->velocity += curr.particle->acceleration;
                    curr.particle->update(state);
                }
            }
        }
    } else {
        for (int y = state.height - 1; y >= 0; y--) {
            for (int x = state.height - 1; x >= 0; x--) {
                Cell& curr = state.getElement(x, y);
                if (curr.type == CellType::PARTICLE) {
                    curr.particle->velocity += curr.particle->acceleration;
                    curr.particle->update(state);
                }
            }
        }
    }
}

void GameWorld::resetGrid() {
    for (int y = 0; y < state.height; y++) {
        for (int x = 0; x < state.width; x++) {
            Cell& curr = state.getElement(x, y);
            curr.unsetParticle();
        }
    }
}