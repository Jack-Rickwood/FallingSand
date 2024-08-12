#include "game.h"

void Game::start() {
    switch (mode) {
    case GameMode::SURVIVAL: {
        std::thread generationThread(&WorldGenerator::generate, &world_generator, std::ref(world));
        renderer.showWorldGenProgress(world_generator, input_handler);
        generationThread.join();

        sf::Vector2i player_pos = player.getPos();
        for (int y = 0; y < world_height; y++) {
            if (world.getState().getElement(player_pos.x, y).type != CellType::EMPTY) {
                player.setPos({player_pos.x, y});
                camera_pos = player.getPos();
                break;
            }
        }
        loop(true);
        break;
    }
    case GameMode::CREATIVE: {
        std::thread generationThread(&WorldGenerator::generate, &world_generator, std::ref(world));
        renderer.showWorldGenProgress(world_generator, input_handler);
        generationThread.join();

        loop(true);
        break;
    }
    case GameMode::MAP_PREVIEW: {
        std::thread generationThreadPreview(&WorldGenerator::generate, &world_generator, std::ref(world));
        renderer.showWorldGenProgress(world_generator, input_handler);
        generationThreadPreview.join();

        loop(false);
        break;
    }
    default: {
        break;
    }
    }
}

void Game::loop(bool evolve_world) {
    while (renderer.windowIsOpen()) {
        switch (mode) {
        case GameMode::SURVIVAL: {
            input_handler.handleEventsSurvival(renderer.getWindow(), world, player, camera_pos);
            player.update(world);
            break;
        }
        case GameMode::CREATIVE: {
            input_handler.handleEventsCreative(renderer.getWindow(), world, renderer.getWorldPosOfMouse(camera_pos), camera_pos);
            break;
        }
        case GameMode::MAP_PREVIEW: {
            input_handler.handleEventsPreview(renderer.getWindow(), world, renderer.getWorldPosOfMouse(camera_pos), camera_pos);
            break;
        }
        }


        sf::Vector2i view_size = sf::Vector2i(renderer.getViewSize());
        if (evolve_world) {
            world.evolve(frame_num, camera_pos.x, camera_pos.y, view_size.x * 2.0f, view_size.y * 2.0f);
        }

        frame_time = clock.getElapsedTime().asSeconds();
        input_handler.updateBrushCooldown(frame_time);
        fps = 1.0f / frame_time;
        clock.restart();

        int x1 = std::max((int)(camera_pos.x - (view_size.x/2)), 0);
        int y1 = std::max((int)(camera_pos.y - (view_size.y/2)), 0);
        int x2 = std::min((int)(camera_pos.x + (view_size.x/2)), world_width - 1);
        int y2 = std::min((int)(camera_pos.y + (view_size.y/2)), world_height - 1);
        renderer.render(world, player, x1, y1, x2, y2, fps, input_handler.getBrushType());

        if (mode == GameMode::SURVIVAL) {
            camera_pos = player.getPos() + sf::Vector2i(0, -view_size.y/4);
        }

        frame_num++;
    }
}