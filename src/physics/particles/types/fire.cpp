#include "../../containers/grid.h"

void Fire::particleInteractions(GameGrid& state) {
    std::vector<sf::Vector2i> moves;

    moves.push_back(state.traverse(position, {-1, 0}));
    moves.push_back(state.traverse(position, {1, 0}));
    moves.push_back(state.traverse(position, {0, -1}));
    moves.push_back(state.traverse(position, {0, 1}));

    moves.push_back(state.traverse(position, {-1, -1}));
    moves.push_back(state.traverse(position, {1, -1}));
    moves.push_back(state.traverse(position, {-1, 1}));
    moves.push_back(state.traverse(position, {1, 1}));

    constexpr int upd_per_sec = 12;
    bool burn = false;

    for (int i = 0; i < moves.size(); i++) {
        Cell& target = state.getElement(moves[i]);
        
        if (target.type == CellType::PARTICLE) {
            float prox_adj = (i < 4 ? 1.0f : 0.25f);
            float final_adj = calculateTempTransfer(temperature, target.particle->getTemperature(), target.particle->getFlammability(), prox_adj, upd_per_sec);

            if (final_adj > 0.0f) {
                target.particle->setTemperature(target.particle->getTemperature() + final_adj);
                burn = true;
            }

            if (target.particle->getTemperature() > 100.0f) {
                Fire spread(target.particle->getPosition());
                target.setParticle(std::make_shared<Fire>(spread));
            }
        }
    }

    if (!burn) {
        extinguish_ct++;
    }

    if (extinguish_ct > 60 || temperature < 100.0f) {
        Cell& curr = state.getElement(position);
        Smoke repl(position);
        curr.setParticle(std::make_shared<Smoke>(repl));
        curr.particle->setAcceleration(curr.particle->getGravity());
    }
}

void Ember::particleInteractions(GameGrid& state) {
    std::vector<sf::Vector2i> moves;

    moves.push_back(state.traverse(position, {-1, 0}));
    moves.push_back(state.traverse(position, {1, 0}));
    moves.push_back(state.traverse(position, {0, -1}));
    moves.push_back(state.traverse(position, {0, 1}));

    constexpr int upd_per_sec = 24;

    for (int i = 0; i < moves.size(); i++) {
        Cell& target = state.getElement(moves[i]);
        
        if (target.type == CellType::PARTICLE) {
            float prox_adj = (i < 4 ? 1.0f : 0.25f);
            float final_adj = calculateTempTransfer(temperature, target.particle->getTemperature(), target.particle->getFlammability(), prox_adj, upd_per_sec);

            target.particle->setTemperature(target.particle->getTemperature() + final_adj);

            if (target.particle->getTemperature() > 100.0f) {
                Fire spread(target.particle->getPosition());
                target.setParticle(std::make_shared<Fire>(spread));
            }
        }
    }
}