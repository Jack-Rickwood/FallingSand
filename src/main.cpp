#include <iostream>
#include <SFML/Graphics.hpp>
#include "graphics/renderer.h"

#define WORLD_WIDTH 300
#define WORLD_HEIGHT 300
#define FRAMERATE 60

int main(int argc, const char *argv[]) {
    Renderer renderer(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, FRAMERATE, WORLD_WIDTH, WORLD_HEIGHT);
    renderer.loop();

    return 0;
}