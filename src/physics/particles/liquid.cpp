#include "../containers/grid.h"

void Liquid::disperse(GameGrid& state, sf::Vector2i dir) {
    std::vector<sf::Vector2i> moves;
    sf::Vector2i curr_pos = position;
    int disperse_rate = 7;
    for (int i = 0; i < disperse_rate; i++) {
        moves.push_back(state.traverse(curr_pos, dir));
        curr_pos = moves[i];
    }

    for (int i = 0; i < moves.size(); i++) {
        Cell& curr = state.getElement(position);
        Cell& dest = state.getElement(moves[i]);

        if (dest.type == CellType::EMPTY) {
            state.moveParticle(curr, dest, moves[i]);
            continue;
        } else {
            if (dest.particle->getForm() == ParticleForm::SOLID) {
                break;
            } else if (dest.particle->getForm() == ParticleForm::LIQUID) {
                break;
            } else if (dest.particle->getForm() == ParticleForm::GAS) {
                state.swapParticle(curr, dest);
                continue;
            }
        }
    }
}

void Liquid::update(GameGrid& state) {
    if (!particle_mode) {
        std::vector<sf::Vector2i> moves;
        moves.push_back(state.traverse(position, {0, 1}));
        moves.push_back(state.traverse(position, {-1, 1}));
        moves.push_back(state.traverse(position, {1, 1}));
        int dir_adj = Rand::range(0, 1) ? 1 : -1;
        moves.push_back(state.traverse(position, dir_adj * sf::Vector2i{1, 0}));

        for (int i = 0; i < moves.size(); i++) {
            Cell& curr = state.getElement(position);
            Cell& dest = state.getElement(moves[i]);

            if (dest.type == CellType::EMPTY) {
                sf::Vector2i diff = moves[i] - position;
                if (diff.y == 0) {
                    disperse(state, diff);
                } else {
                    state.moveParticle(curr, dest, moves[i]);
                    particle_mode = true;
                }
                break;
            } else {
                if (dest.particle->getForm() == ParticleForm::SOLID) {
                    continue;
                } else if (dest.particle->getForm() == ParticleForm::LIQUID) {
                    if (getDensity() > dest.particle->getDensity()) {
                        state.swapParticle(curr, dest);
                        break;
                    } else {
                        continue;
                    }
                } else if (dest.particle->getForm() == ParticleForm::GAS) {
                    state.swapParticle(curr, dest);
                    break;
                }
            }
        }
    } else {
        particle_mode = applyVelocity(state);
    }
    particleInteractions(state);
}