#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../engine/game_world.h"

enum class ParticleType {
    SAND,
    WATER,
    DIRT,
    STONE,
    ACID,
    SMOKE,
    EMBER,
    WOOD
};

enum class BrushSize {
    SMALL,
    LARGE,
    SPARSE
};

class Renderer {
public:
    sf::RenderWindow window;
    sf::View view;
    GameWorld world;

    sf::Texture main_texture, bloom_texture;
    sf::RenderTexture bloom_mask, brightpass_texture, horizontal_blur_texture, vertical_blur_texture;
    sf::Shader brightpass_shader, blur_shader, bloom_shader;

    int win_width;
    int win_height;
    int aspect_ratio;
    int framerate;
    int world_width;
    int world_height;

    sf::Clock clock;
    float brush_cooldown = 0;
    float frame_time = 0;
    int fps = 0;

    ParticleType active = ParticleType::SAND;
    BrushSize brush_size = BrushSize::LARGE;

    bool left_mouse_down = false;

    sf::Font font;

    Renderer(int win_width_, int win_height_, int framerate_, int world_width_, int world_height_) :
        window(sf::VideoMode(win_width_, win_height_), "PhysicsEngine", sf::Style::Default),
        world({world_width_, world_height_}),
        view(sf::Vector2f(world_width_/2, world_height_/2), sf::Vector2f(world_width_, world_height_)),
        world_width(world_width_),
        world_height(world_height_),
        framerate(framerate_),
        win_width(win_width_),
        win_height(win_height_)
    {
        aspect_ratio = win_width_ / win_height_;
        window.setFramerateLimit(framerate_);
        float rel_width = 0.5f;
        float rel_height = ((float)win_width_ * rel_width) / (float)win_height_;
        view.setViewport(sf::FloatRect((1.0f - rel_width) / 2.0f, (1.0f - rel_height) / 2.0f, rel_width, rel_height));
        window.setView(view);
        font.loadFromFile("res/Roboto-Regular.ttf");

        main_texture.create(world_width, world_height);
        bloom_texture.create(world_width, world_height);

        brightpass_shader.loadFromFile("res/vertex.glsl", "res/brightpass.glsl");
        blur_shader.loadFromFile("res/vertex.glsl", "res/blur.glsl");
        bloom_shader.loadFromFile("res/vertex.glsl", "res/bloom.glsl");

        bloom_mask.create(world_width, world_height);
        brightpass_texture.create(world_width, world_height);
        horizontal_blur_texture.create(world_width, world_height);
        vertical_blur_texture.create(world_width, world_height);
    }

    void render();
    void renderGrid();
    void renderHUD();
    void spawnParticle();
    void handleEvents();
    void loop();
};