#pragma once
#include "../physics/containers/grid.h"
#include "../physics/containers/cell.h"

class GameWorld {
private:
    GameGrid state;

public:
    GameWorld(sf::Vector2i size) :
        state(size.x, size.y)
    {}

    void evolve(int frame_num);
    void addParticle(std::shared_ptr<Particle> ptcl);
    void updateParticles(int frame_num);
    void resetGrid();

    GameGrid& getState() {    return state;   }
};