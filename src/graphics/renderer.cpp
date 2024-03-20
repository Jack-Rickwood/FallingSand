#include "renderer.h"

void Renderer::render() {
    renderGrid();
    renderHUD();
}

void Renderer::renderGrid() {
    sf::Image main_layer, bloom_layer;
    main_layer.create(world_width, world_height, sf::Color::Transparent);
    bloom_layer.create(world_width, world_height, sf::Color::Transparent);

    auto& state = world.getState();
    for (int y = 0; y < state.height; y++) {
        for (int x = 0; x < state.width; x++) {
            Cell& cell = state.getElement(x, y);
            if (cell.type == CellType::PARTICLE) {
                if (cell.particle->enableBloom()) {
                    bloom_layer.setPixel(x, y, cell.particle->color);
                } else {
                    main_layer.setPixel(x, y, cell.particle->color);
                }
            }
        }
    }

    main_texture.loadFromImage(main_layer);
    bloom_texture.loadFromImage(bloom_layer);

    bloom_mask.clear();
    bloom_mask.draw(sf::Sprite(bloom_texture));
    bloom_mask.display();

    brightpass_shader.setUniform("texture", bloom_mask.getTexture());

    brightpass_texture.clear();
    brightpass_texture.draw(sf::Sprite(bloom_mask.getTexture()), &brightpass_shader);
    brightpass_texture.display();

    blur_shader.setUniform("image", brightpass_texture.getTexture());
    blur_shader.setUniform("horizontal", true);

    horizontal_blur_texture.clear();
    horizontal_blur_texture.draw(sf::Sprite(brightpass_texture.getTexture()), &blur_shader);
    horizontal_blur_texture.display();

    blur_shader.setUniform("image", horizontal_blur_texture.getTexture());
    blur_shader.setUniform("horizontal", false);

    vertical_blur_texture.clear();
    vertical_blur_texture.draw(sf::Sprite(horizontal_blur_texture.getTexture()), &blur_shader);
    vertical_blur_texture.display();

    bloom_shader.setUniform("original", bloom_mask.getTexture());
    bloom_shader.setUniform("blurred", vertical_blur_texture.getTexture());

    sf::RenderStates states;
    states.shader = &bloom_shader;
    states.blendMode = sf::BlendAdd;

    window.draw(sf::Sprite(main_texture));
    window.draw(sf::Sprite(bloom_mask.getTexture()), states);
}

void Renderer::renderHUD() {
    sf::Text ptcl_text("Particle: ", font, 30);
    ptcl_text.setFillColor(sf::Color::White);
    ptcl_text.setPosition({20.0f, 10.0f});
    ptcl_text.setStyle(sf::Text::Bold);
    switch (active) {
    case ParticleType::SAND:
        ptcl_text.setString("Particle: Sand");
        break;
    case ParticleType::WATER:
        ptcl_text.setString("Particle: Water");
        break;
    case ParticleType::DIRT:
        ptcl_text.setString("Particle: Dirt");
        break;
    case ParticleType::STONE:
        ptcl_text.setString("Particle: Stone");
        break;
    case ParticleType::ACID:
        ptcl_text.setString("Particle: Acid");
        break;
    case ParticleType::SMOKE:
        ptcl_text.setString("Particle: Smoke");
        break;
    case ParticleType::EMBER:
        ptcl_text.setString("Particle: Ember");
        break;
    case ParticleType::WOOD:
        ptcl_text.setString("Particle: Wood");
        break;
    default:
        break;
    }

    int brush_radius = 0;
    sf::Text size_text("Brush Size: ", font, 30);
    size_text.setFillColor(sf::Color::White);
    size_text.setPosition({20.0f, 60.0f});
    size_text.setStyle(sf::Text::Bold);
    switch (brush_size) {
    case BrushSize::SMALL:
        size_text.setString("Brush Size: Small");
        brush_radius = 1;
        break;
    case BrushSize::LARGE:
        size_text.setString("Brush Size: Large");
        brush_radius = 8;
        break;
    case BrushSize::SPARSE:
        size_text.setString("Brush Size: Sparse");
        brush_radius = 8;
        break;
    default:
        break;
    }

    sf::Text fps_text("FPS: ", font, 30);
    fps_text.setFillColor(sf::Color::White);
    fps_text.setPosition({win_width - 120.0f, 10.0f});
    fps_text.setStyle(sf::Text::Bold);
    fps_text.setString("FPS: " + std::to_string(fps));

    auto temp = window.getView();
    window.setView(window.getDefaultView());
    window.draw(ptcl_text);
    window.draw(size_text);
    window.draw(fps_text);
    window.setView(temp);

    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2i world_pos = sf::Vector2i{window.mapPixelToCoords(mouse_pos)} - sf::Vector2i{brush_radius, brush_radius};
    sf::CircleShape cursor(brush_radius, 30);
    cursor.setOutlineColor(sf::Color::White);
    cursor.setOutlineThickness(1);
    cursor.setFillColor(sf::Color::Transparent);
    cursor.setPosition(world_pos.x, world_pos.y);
    window.draw(cursor);
}


void Renderer::spawnParticle() {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    sf::Vector2i world_pos = sf::Vector2i{window.mapPixelToCoords(mouse_pos)};

    int num = 1;
    int gap = 1;
    if (brush_size == BrushSize::SMALL) {
        num = 1;
    } else if (brush_size == BrushSize::LARGE) {
        num = 16;
    } else if (brush_size == BrushSize::SPARSE) {
        num = 16;
        gap = 2;
    }

    int radius = num / 2;
    sf::Vector2i center = world_pos - sf::Vector2i{radius, radius};
    for (int y = 0; y < num; y++) {
        for (int x = 0; x < num; x++) {
            int dx = x - radius;
            int dy = y - radius;
            if (dx * dx + dy * dy <= radius * radius) {
                if (x % gap == 0 && y % gap == 0) {
                    if (active == ParticleType::SAND) {
                        Sand ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Sand>(ptcl));
                    } else if (active == ParticleType::WATER) {
                        Water ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Water>(ptcl));
                    } else if (active == ParticleType::DIRT) {
                        Dirt ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Dirt>(ptcl));
                    } else if (active == ParticleType::STONE) {
                        Stone ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Stone>(ptcl));
                    } else if (active == ParticleType::ACID) {
                        Acid ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Acid>(ptcl));
                    } else if (active == ParticleType::SMOKE) {
                        Smoke ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Smoke>(ptcl));
                    } else if (active == ParticleType::EMBER) {
                        Ember ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Ember>(ptcl));
                    } else if (active == ParticleType::WOOD) {
                        Wood ptcl(center + sf::Vector2i{x, y});
                        world.addParticle(std::make_shared<Wood>(ptcl));
                    }
                }
            }
        }
    }
}

void Renderer::handleEvents() {
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
                active = ParticleType::SAND;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                active = ParticleType::WATER;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                active = ParticleType::DIRT;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                active = ParticleType::STONE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                active = ParticleType::ACID;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                active = ParticleType::SMOKE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                active = ParticleType::EMBER;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                active = ParticleType::WOOD;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
                brush_size = BrushSize::SMALL;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
                brush_size = BrushSize::LARGE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
                brush_size = BrushSize::SPARSE;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
                world.resetGrid();
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
        spawnParticle();
    }
}

void Renderer::loop() {
    int frame_num = 0;

    while (window.isOpen()) {
        handleEvents();

        world.evolve(frame_num);

        frame_time = clock.getElapsedTime().asSeconds();
        brush_cooldown -= frame_time;
        fps = (int)(1.0f / frame_time);
        clock.restart();

        window.clear(sf::Color::Black);
        render();
        window.display();

        frame_num++;
    }
}