#include "../../containers/grid.h"

void Acid::particleInteractions(GameGrid& state) {
    std::vector<sf::Vector2i> moves;
    moves.push_back(state.traverse(position, {0, 1}));
    moves.push_back(state.traverse(position, {-1, 1}));
    moves.push_back(state.traverse(position, {1, 1}));

    for (int i = 0; i < moves.size(); i++) {
        Cell& dest = state.getElement(moves[i]);
        if (dest.type == CellType::PARTICLE) {
            if (dest.particle->getForm() == ParticleForm::SOLID) {
                float destroy_chance = (0.01f * (float)Rand::range(1, 100));
                bool frame_chance = Rand::range(1, 6) == 6;
                bool destroy = getCorrosivity() > destroy_chance && frame_chance;

                if (destroy) {
                    dest.unsetParticle();
                }
            }
        }
    }
}