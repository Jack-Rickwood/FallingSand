#include "renderer.h"

sf::Vector2i Renderer::getWorldPosOfMouse(sf::Vector2i camera_pos) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
    return sf::Vector2i{window.mapPixelToCoords(mouse_pos)} + camera_pos - sf::Vector2i{0.5f * view.getSize()};
}

void Renderer::render(GameWorld& world, Player& player, int x1, int y1, int x2, int y2, int fps, InputHandler::BrushType brush_type) {
    window.clear(sf::Color::Black);
    renderGrid(world, player, x1, y1, x2, y2);
    renderHUD(fps, brush_type);
    window.display();
}

void Renderer::renderGrid(GameWorld& world, Player& player, int x1, int y1, int x2, int y2) {
    sf::Image main_layer, bloom_layer;
    main_layer.create(view.getSize().x, view.getSize().y, sf::Color::Transparent);
    bloom_layer.create(view.getSize().x, view.getSize().y, sf::Color::Transparent);

    auto& state = world.getState();
    for (int y = y1; y < y2; y++) {
        for (int x = x1; x < x2; x++) {
            Cell& cell = state.getElement(x, y);
            if (cell.type == CellType::PARTICLE) {
                if (cell.particle->enableBloom()) {
                    bloom_layer.setPixel(x - x1, y - y1, cell.particle->getColor());
                } else {
                    main_layer.setPixel(x - x1, y - y1, cell.particle->getColor());
                }
            }
        }
    }

    renderPlayer(player, main_layer, x1, y1);

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

void Renderer::renderPlayer(Player& player, sf::Image& main_layer, int x1, int y1) {
    std::array<std::array<sf::Color, CHAR_WIDTH>, CHAR_HEIGHT> particles = player.getParticles();
    for (int x = 0; x < CHAR_WIDTH; x++) {
        for (int y = 0; y < CHAR_HEIGHT; y++) {
            sf::Vector2i image_pos = player.getPos() + sf::Vector2i{CHAR_WIDTH / 2, -CHAR_HEIGHT} + sf::Vector2i{x, y} - sf::Vector2i{x1, y1};
            if (image_pos.x >= 0 && image_pos.x < main_layer.getSize().x && image_pos.y >= 0 && image_pos.y < main_layer.getSize().y) {
                main_layer.setPixel(image_pos.x, image_pos.y, particles[y][x]);
            }
        }
    }
}

void Renderer::renderHUD(int fps, InputHandler::BrushType brush_type) {
    sf::Text ptcl_text("Particle: ", font, 30);
    ptcl_text.setFillColor(sf::Color::White);
    ptcl_text.setPosition({20.0f, 10.0f});
    ptcl_text.setStyle(sf::Text::Bold);
    switch (brush_type.particle) {
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
    switch (brush_type.size) {
    case InputHandler::BrushSize::SMALL:
        size_text.setString("Brush Size: Small");
        brush_radius = 1;
        break;
    case InputHandler::BrushSize::LARGE:
        size_text.setString("Brush Size: Large");
        brush_radius = 8;
        break;
    case InputHandler::BrushSize::SPARSE:
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

void Renderer::showWorldGenProgress(WorldGenerator& world_generator, InputHandler& input_handler) {
    // Set the default view to correctly position the progress bar and text
    window.setView(window.getDefaultView());

    // Calculate the position for the progress bar
    sf::Vector2f position = sf::Vector2f((win_width - 600) / 2, (win_height + 50) / 2);

    // Create the progress bar background
    sf::RectangleShape progressBarBackground(sf::Vector2f(600, 50));
    progressBarBackground.setFillColor(sf::Color(50, 50, 50));
    progressBarBackground.setPosition(position);

    // Create the progress bar
    sf::RectangleShape progressBar(sf::Vector2f(0, 50));
    progressBar.setFillColor(sf::Color(100, 250, 50));
    progressBar.setPosition(position);

    // Load the font and create the text
    sf::Font font;
    if (!font.loadFromFile("res/Roboto-Regular.ttf")) {
        // Handle error
        return;
    }
    sf::Text progressText("Generating world...", font, 24);
    progressText.setFillColor(sf::Color::White);
    progressText.setPosition(position.x, position.y - 40); // Position above the progress bar

    int progress = 0;
    while (progress < 100) {
        input_handler.handleEventsLoadingScreen(window);

        progressBar.setSize(sf::Vector2f(6 * progress, 50));

        window.clear(sf::Color::Black);
        window.draw(progressBarBackground);
        window.draw(progressBar);
        window.draw(progressText); // Draw the progress text
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Adjust as needed

        progress = world_generator.getGenProgress().load();
    }

    // Restore the original view
    window.setView(view);
}
