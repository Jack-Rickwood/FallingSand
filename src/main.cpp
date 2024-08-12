#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "engine/game.h"

#define WORLD_WIDTH 5000
#define WORLD_HEIGHT 1500
#define FRAMERATE 60

int main(int argc, const char *argv[]) {
    //Game game(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, FRAMERATE, WORLD_WIDTH, WORLD_HEIGHT, 0.2f, Game::GameMode::CREATIVE, 567954, 0.5f);
    
    Game game(
        sf::VideoMode::getDesktopMode().width,
        sf::VideoMode::getDesktopMode().height,
        FRAMERATE, WORLD_WIDTH, WORLD_HEIGHT,
        (float)WORLD_HEIGHT / (float)sf::VideoMode::getDesktopMode().height,
        Game::GameMode::MAP_PREVIEW,
        4907325,
        0.5f
    );
    
    
    
    game.start();

    return 0;
}