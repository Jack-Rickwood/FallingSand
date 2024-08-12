#pragma once
#include "../../physics/containers/grid.h"
#include "../../physics/containers/cell.h"
#include "../../physics/utils/rng.h"

class GameWorld {
public:
    GameWorld(sf::Vector2i size) :
        world_size(size),
        state(size.x, size.y)
    {}

    template<typename T>
    static std::shared_ptr<T> createParticle(int x, int y) {
        return std::make_shared<T>(sf::Vector2i(x, y));
    }

    void evolve(int frame_num, int cx, int cy, int upd_width, int upd_height);
    void addParticle(std::shared_ptr<Particle> ptcl);
    void updateSection(int frame_num, int chunk_size, int x1, int y1, int x2, int y2);
    void updateCheckerboard(int frame_num, std::vector<std::pair<int, int>>& chunks, int chunk_size, int y1, int y2, bool first);
    void updateChunk(int frame_num, int x1, int y1, int x2, int y2);
    void updateCell(int frame_num, int x, int y);
    void resetGrid();

    sf::Vector2i getSize() { return world_size; }
    GameGrid& getState() { return state; }

private:
    sf::Vector2i world_size;
    GameGrid state;
};