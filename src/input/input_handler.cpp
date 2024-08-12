#include "input_handler.h"

void InputHandler::spawnParticle(GameWorld& world, sf::Vector2i world_pos) {
    int num, gap;
    switch (brush_type.size) {
    case BrushSize::SMALL:
        num = 1;
        gap = 1;
        break;
    case BrushSize::LARGE:
        num = 16;
        gap = 1;
        break;
    case BrushSize::SPARSE:
        num = 16;
        gap = 2;
        break;
    }

    int radius = num / 2;
    sf::Vector2i center = world_pos - sf::Vector2i{radius, radius};
    for (int y = 0; y < num; y++) {
        for (int x = 0; x < num; x++) {
            int dx = x - radius;
            int dy = y - radius;
            if (dx * dx + dy * dy <= radius * radius) {
                if (x % gap == 0 && y % gap == 0) {
                    switch (brush_type.particle) {
                    case ParticleType::SAND: {
                        Sand ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Sand>(ptcl));
                        break;
                    }
                    case ParticleType::WATER: {
                        Water ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Water>(ptcl));
                        break;
                    }
                    case ParticleType::DIRT: {
                        Dirt ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Dirt>(ptcl));
                        break;
                    }
                    case ParticleType::STONE: {
                        Stone ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Stone>(ptcl));
                        break;
                    }
                    case ParticleType::ACID: {
                        Acid ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Acid>(ptcl));
                        break;
                    }
                    case ParticleType::SMOKE: {
                        Smoke ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Smoke>(ptcl));
                        break;
                    }
                    case ParticleType::EMBER: {
                        Ember ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Ember>(ptcl));
                        break;
                    }
                    case ParticleType::WOOD: {
                        Wood ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Wood>(ptcl));
                        break;
                    }
                    default:
                        // Handle any other cases here
                        break;
                }
                }
            }
        }
    }
}

void InputHandler::handleEventsCreative(sf::Window& window, GameWorld& world, sf::Vector2i world_pos, sf::Vector2i& camera_pos) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                brush_type.particle = ParticleType::SAND;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                brush_type.particle = ParticleType::WATER;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                brush_type.particle = ParticleType::DIRT;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                brush_type.particle = ParticleType::STONE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                brush_type.particle = ParticleType::ACID;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                brush_type.particle = ParticleType::SMOKE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                brush_type.particle = ParticleType::EMBER;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                brush_type.particle = ParticleType::WOOD;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
                brush_type.size = BrushSize::SMALL;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
                brush_type.size = BrushSize::LARGE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
                brush_type.size = BrushSize::SPARSE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
                world.resetGrid();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                camera_pos.x -= 10;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                camera_pos.x += 10;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                camera_pos.y -= 10;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                camera_pos.y += 10;
            }
            break;
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                left_mouse_down = true;
            }
            break;
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                left_mouse_down = false;
            }
            break;
        default:
            break;
        }
    }

    if (left_mouse_down && brush_cooldown < 0) {
        spawnParticle(world, world_pos);
    }
}

void InputHandler::handleEventsSurvival(sf::Window& window, GameWorld& world, Player& player, sf::Vector2i& camera_pos) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                player.moveLeft();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                player.moveRight();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                player.jump();
            }
            break;
        default:
            break;
        }
    }
}

void InputHandler::handleEventsPreview(sf::Window& window, GameWorld& world, sf::Vector2i world_pos, sf::Vector2i& camera_pos) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                camera_pos.x -= 10;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                camera_pos.x += 10;
            }
            break;
        default:
            break;
        }
    }
}

void InputHandler::handleEventsLoadingScreen(sf::Window& window) {
    sf::Event event{};
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        default:
            break;
        }
    }
}