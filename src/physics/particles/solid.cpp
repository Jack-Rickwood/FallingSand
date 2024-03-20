#include "../containers/grid.h"

void MovableSolid::update(GameGrid& state) {
    if (!particle_mode) {
        std::vector<sf::Vector2i> moves;
        moves.push_back(state.traverse(position, {0, 1}));
        moves.push_back(state.traverse(position, {-1, 1}));
        moves.push_back(state.traverse(position, {1, 1}));

        for (int i = 0; i < moves.size(); i++) {
            Cell& curr = state.getElement(position);
            Cell& dest = state.getElement(moves[i]);
            sf::Vector2i diff = moves[i] - position;

            if (dest.type == CellType::EMPTY) {
                if (diff.x == 0) {
                    state.moveParticle(curr, dest, moves[i]);
                    particle_mode = true;
                } else {
                    if (!stuck) {
                        stuck = getResistance() > (0.01f * (float)Rand::range(1, 100));
                        state.moveParticle(curr, dest, moves[i]);
                    }
                }
                break;
            } else {
                if (dest.particle->getForm() == ParticleForm::SOLID) {
                    continue;
                } else if (dest.particle->getForm() == ParticleForm::LIQUID) {
                    if (diff.x == 0) {
                        state.swapParticle(curr, dest);
                        break;
                    } else {
                        if (!stuck) {
                            stuck = dest.particle->getResistance() > (0.01f * (float)Rand::range(1, 100));
                            state.swapParticle(curr, dest);
                        }
                        break;
                    }
                } else if (dest.particle->getForm() == ParticleForm::GAS) {
                    if (diff.x == 0) {
                        state.swapParticle(curr, dest);
                        continue;
                    } else {
                        break;
                    }
                }
            }
        }
    } else {
        particle_mode = applyVelocity(state);
    }
    particleInteractions(state);
}

void ImmovableSolid::update(GameGrid& state) {
    particleInteractions(state);
}