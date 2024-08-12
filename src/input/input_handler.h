#pragma once
#include <SFML/Graphics.hpp>
#include "../engine/world/game_world.h"
#include "../physics/particle_types.h"
#include "../engine/player.h"

class InputHandler {
public:
    enum class BrushSize {
        SMALL,
        LARGE,
        SPARSE
    };

    struct BrushType {
        ParticleType particle;
        BrushSize size;
    };

    void spawnParticle(GameWorld& world, sf::Vector2i world_pos);
    void handleEventsCreative(sf::Window& window, GameWorld& world, sf::Vector2i world_pos, sf::Vector2i& camera_pos);
    void handleEventsSurvival(sf::Window& window, GameWorld& world, Player& player, sf::Vector2i& camera_pos);
    void handleEventsPreview(sf::Window& window, GameWorld& world, sf::Vector2i world_pos, sf::Vector2i& camera_pos);
    void handleEventsLoadingScreen(sf::Window& window);

    void updateBrushCooldown(float frame_time) { brush_cooldown -= frame_time; }

    BrushType getBrushType() { return { brush_type }; }

private:
    float brush_cooldown = 0;
    bool left_mouse_down = false;

    BrushType brush_type = {
        ParticleType::SAND,
        BrushSize::LARGE
    };
};