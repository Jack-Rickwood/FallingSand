#include <algorithm>
#include <cmath>
#include "containers/grid.h"

/*========== Update Overrides ==========*/
void Particle::update(GameGrid& state) {
    applyVelocity(state);
}

void Particle::particleInteractions(GameGrid& state) {

}

/*========== Helper Functions ==========*/
std::vector<sf::Vector2i> Particle::getPath(sf::Vector2i pos, sf::Vector2i move) {
    sf::Vector2i dest = pos + move;

    std::vector<sf::Vector2i> path;

    if (pos == dest) {
        return path;
    }

    float dist = sqrt(move.x * move.x + move.y * move.y);
    sf::Vector2f norm_vel = sf::Vector2f{move} / dist;

    sf::Vector2f curr = sf::Vector2f{pos};
    sf::Vector2i next(0, 0);
    while (next != dest) {
        curr += norm_vel;
        next = sf::Vector2i(round(curr.x), round(curr.y));
        path.push_back(next);
    }
    return path;
}

bool Particle::applyVelocity(GameGrid& state) {
    std::vector<sf::Vector2i> path = getPath(position, velocity);
    for (int i = 0; i < path.size(); i++) {
        if (position != path[i]) {
            if (state.checkValidIndex(path[i])) {
                Cell& curr = state.getElement(position);
                Cell& move = state.getElement(path[i]);
                if (move.type == CellType::EMPTY) {
                    state.moveParticle(curr, move, path[i]);
                } else {
                    velocity = {0, 0};
                    return false;
                }
            } else {
                velocity = {0, 0};
                return false;
            }
        }
    }
    return true;
}

float Particle::calculateTempTransfer(float temp_curr, float temp_targ, float flam, float prox_adj, int upd_per_sec) {
    float flam_adj = flam*flam;
    float temp_adj = (temp_curr - temp_targ) * 0.2f;
    temp_adj = (temp_adj > 0.0f && temp_adj < 1.0f ? 1.0f : temp_adj);

    return (prox_adj * flam_adj * temp_adj) / (60.0f / (float)upd_per_sec);
}