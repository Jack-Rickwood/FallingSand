#include <memory>
#include <thread>
#include "game_world.h"

void GameWorld::evolve(int frame_num, int cx, int cy, int upd_width, int upd_height) {
    int chunk_size = 16;
    int x1 = std::max(cx - (upd_width/2) - chunk_size, 0);
    int y1 = std::max(cy - (upd_height/2), 0);
    int x2 = std::min(cx + (upd_width/2) + chunk_size, state.width - 1);
    int y2 = std::min(cy + (upd_height/2), state.height - 1);
    updateSection(frame_num, chunk_size, x1, y1, x2, y2);
}

void GameWorld::addParticle(std::shared_ptr<Particle> ptcl) {
    if (state.checkValidIndex(ptcl->getPosition())) {
        state.getElement(ptcl->getPosition()).setParticle(ptcl);
        ptcl->setAcceleration(ptcl->getGravity());
    }
}

void GameWorld::updateSection(int frame_num, int chunk_size, int x1, int y1, int x2, int y2) {
    // Divide the map into chunks
    int rand_offset = Rand::range(-chunk_size, chunk_size);
    std::vector<std::pair<int, int>> chunks;
    for (int x = x1; x < x2; x += chunk_size) {
        chunks.push_back({x + rand_offset, std::min(x + rand_offset + chunk_size - 1, state.width - 1)});
    }
    
    updateCheckerboard(frame_num, chunks, chunk_size, y1, y2, true);
    updateCheckerboard(frame_num, chunks, chunk_size, y1, y2, false);
}

void GameWorld::updateCheckerboard(int frame_num, std::vector<std::pair<int, int>>& chunks, int chunk_size, int y1, int y2, bool first) {
    std::vector<std::thread> threads;
    for (size_t i = 0; i < chunks.size(); i++) {
        int x1 = std::max(chunks[i].first, 0);
        int x2 = std::max(chunks[i].second, 0);

        if (first) {
            if ((x1 / chunk_size) % 2 == 0) {
                threads.emplace_back(&GameWorld::updateChunk, this, frame_num, x1, y1, x2, y2);
            }
        } else {
            if ((x1 / chunk_size) % 2 != 0) {
                threads.emplace_back(&GameWorld::updateChunk, this, frame_num, x1, y1, x2, y2);
            }
        }
    }

    // Join threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads.clear();
}

void GameWorld::updateChunk(int frame_num, int x1, int y1, int x2, int y2) {
    bool flip = frame_num % 2 == 0;
    if (flip) {
        for (int y = y2; y >= y1; y--) {
            for (int x = x1; x <= x2; x++) {
                updateCell(frame_num, x, y);
            }
        }
    } else {
        for (int y = y2; y >= y1; y--) {
            for (int x = x2; x >= x1; x--) {
                updateCell(frame_num, x, y);
            }
        }
    }
}

void GameWorld::updateCell(int frame_num, int x, int y) {
    Cell& curr = state.getElement(x, y);
    /**
     * Remove last_updated from particle
    */
    if (curr.type == CellType::PARTICLE) {
        curr.particle->setVelocity(curr.particle->getVelocity() + curr.particle->getAcceleration());
        curr.particle->update(state);
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