#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../engine/world/game_world.h"
#include "../engine/world/world_generator.h"
#include "../physics/particle_types.h"
#include "../input/input_handler.h"
#include "../engine/player.h"

class Renderer {
public:
    Renderer(int win_width_, int win_height_, int framerate_, float render_scale_) :
        window(sf::VideoMode(win_width_, win_height_), "PhysicsEngine", sf::Style::Default),
        view(
            0.5f * sf::Vector2f(render_scale_ * win_width_, render_scale_ * win_height_),
            sf::Vector2f(render_scale_ * win_width_, render_scale_ * win_height_)
        ),
        render_scale(render_scale_),
        framerate(framerate_),
        win_width(win_width_),
        win_height(win_height_)
    {
        aspect_ratio = win_width_ / win_height_;
        window.setFramerateLimit(framerate_);
        window.setView(view);
        font.loadFromFile("res/Roboto-Regular.ttf");

        main_texture.create(view.getSize().x, view.getSize().y);
        bloom_texture.create(view.getSize().x, view.getSize().y);

        brightpass_shader.loadFromFile("res/vertex.glsl", "res/brightpass.glsl");
        blur_shader.loadFromFile("res/vertex.glsl", "res/blur.glsl");
        bloom_shader.loadFromFile("res/vertex.glsl", "res/bloom.glsl");

        bloom_mask.create(view.getSize().x, view.getSize().y);
        brightpass_texture.create(view.getSize().x, view.getSize().y);
        horizontal_blur_texture.create(view.getSize().x, view.getSize().y);
        vertical_blur_texture.create(view.getSize().x, view.getSize().y);
    }

    sf::Vector2i getWorldPosOfMouse(sf::Vector2i camera_pos);

    void render(GameWorld& world, Player& player, int x1, int y1, int x2, int y2, int fps, InputHandler::BrushType brush_type);
    void renderGrid(GameWorld& world, Player& player, int x1, int y1, int x2, int y2);
    void renderPlayer(Player& player, sf::Image& main_layer, int x1, int y1);
    void renderHUD(int fps, InputHandler::BrushType active_brush);

    sf::Window& getWindow() { return window; }
    bool windowIsOpen() { return window.isOpen(); }
    sf::Vector2f getViewSize() { return view.getSize(); }

    void showWorldGenProgress(WorldGenerator& world_generator, InputHandler& input_handler);

private:
    sf::RenderWindow window;
    sf::View view;

    sf::Texture main_texture, bloom_texture;
    sf::RenderTexture bloom_mask, brightpass_texture, horizontal_blur_texture, vertical_blur_texture;
    sf::Shader brightpass_shader, blur_shader, bloom_shader;

    int render_scale;
    int win_width;
    int win_height;
    int aspect_ratio;
    int framerate;

    sf::Font font;
};