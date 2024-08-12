#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>

#define TRANSPARENT sf::Color::Transparent
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 32

class Player {
private:
    sf::Vector2i old_pos;
    sf::Vector2i pos;
    sf::Vector2i velocity;
    bool on_ground;

    std::array<std::array<sf::Color, CHAR_WIDTH>, CHAR_HEIGHT> particles = {{
        { TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, TRANSPARENT, sf::Color(210,180,140), sf::Color(210,180,140), TRANSPARENT, TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, TRANSPARENT, sf::Color(50,0,0), sf::Color(50,0,0), TRANSPARENT, TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, sf::Color(50,0,0), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(165,42,42), sf::Color(50,0,0), TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), TRANSPARENT, TRANSPARENT, sf::Color(0,0,255), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT },
        { TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT, sf::Color(139,69,19), TRANSPARENT, TRANSPARENT }
    }};

    bool canMoveToPosition(GameWorld& world, sf::Vector2i new_pos) {
        sf::Vector2i size = getSize();
        for (int x = 0; x < size.x; x++) {
            for (int y = 0; y < size.y; y++) {
                sf::Vector2i world_pos = new_pos + sf::Vector2i(size.x / 2, -size.y) + sf::Vector2i(x, y);
                Cell& curr = world.getState().getElement(world_pos);
                if (curr.type != CellType::EMPTY) {
                    return false;
                }
            }
        }
        return true;
    }

    void moveUpSlope(GameWorld& world, int max_step_height) {
        for (int step = 1; step <= max_step_height; step++) {
            if (canMoveToPosition(world, pos + sf::Vector2i(0, -step))) {
                pos.y -= step;
                return;
            }
        }
    }

public:
    Player() :
        pos(0, 0),
        velocity(0, 0),
        on_ground(false)
    {}

    void setPos(sf::Vector2i pos_) {
        pos = pos_;
    }

    sf::Vector2i getPos() {
        return pos;
    }

    void setVelocity(sf::Vector2i velocity_) {
        velocity = velocity_;
    }

    sf::Vector2i getVelocity() {
        return velocity;
    }

    std::array<std::array<sf::Color, CHAR_WIDTH>, CHAR_HEIGHT> getParticles() {
        return particles;
    }

    sf::Vector2i getSize() {
        return { static_cast<int>(particles[0].size()), static_cast<int>(particles.size()) };
    }

    bool doesPlayerOverlap(GameWorld& world) {
        sf::Vector2i size = getSize();
        for (int x = 0; x < size.x; x++) {
            for (int y = 0; y < size.y; y++) {
                sf::Vector2i world_pos = getPos() + sf::Vector2i(size.x / 2, -size.y) + sf::Vector2i(x, y);
                Cell& curr = world.getState().getElement(world_pos);
                if (curr.type != CellType::EMPTY) {
                    return true;
                }
            }
        }
        return false;
    }

    void setPlayerCells(GameWorld& world, bool empty) {
        sf::Vector2i size = getSize();
        for (int x = 0; x < size.x; x++) {
            for (int y = 0; y < size.y; y++) {
                sf::Vector2i world_pos = pos + sf::Vector2i(size.x / 2, -size.y) + sf::Vector2i(x, y);
                Cell& curr = world.getState().getElement(world_pos);
                if (empty) {
                    curr.type = CellType::EMPTY;
                } else {
                    curr.type = CellType::PLAYER;
                }
            }
        }
    }

    void update(GameWorld& world) {
        old_pos = pos;
        setPlayerCells(world, true);

        // Apply gravity
        if (!on_ground) {
            velocity.y = std::min(velocity.y + 1, 10); // Limit fall speed
        }

        // Horizontal movement
        sf::Vector2i new_pos = pos + sf::Vector2i(velocity.x, 0);
        if (canMoveToPosition(world, new_pos)) {
            pos = new_pos;
        } else {
            moveUpSlope(world, 2); // Allow stepping up 2 pixels
        }

        // Vertical movement
        new_pos = pos + sf::Vector2i(0, velocity.y);
        if (canMoveToPosition(world, new_pos)) {
            pos = new_pos;
            on_ground = false;
        } else {
            // Find the ground
            int ground_y = pos.y;
            while (!canMoveToPosition(world, sf::Vector2i(pos.x, ground_y))) {
                ground_y--;
            }
            pos.y = ground_y;
            velocity.y = 0;
            on_ground = true;
        }

        setPlayerCells(world, false);
    }

    void jump() {
        if (on_ground) {
            velocity.y = -10; // Adjust jump strength as needed
            on_ground = false;
        }
    }

    void moveLeft() {
        velocity.x = -2; // Adjust movement speed as needed
    }

    void moveRight() {
        velocity.x = 2; // Adjust movement speed as needed
    }

    void stopHorizontalMovement() {
        velocity.x = 0;
    }
};