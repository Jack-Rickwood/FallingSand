#pragma once
#include <tuple>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>

namespace PColor {
    std::tuple<float, float, float> RGBtoHSV(const sf::Color& color);
    sf::Color HSVtoRGB(float H, float S, float V);
}