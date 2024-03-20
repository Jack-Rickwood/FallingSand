#include "color.h"

namespace PColor {
    // From https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a
    std::tuple<float, float, float> RGBtoHSV(const sf::Color& color) {
        float R = color.r / 255.f;
        float G = color.g / 255.f;
        float B = color.b / 255.f;

        float M = std::max({ R, G, B });
        float m = std::min({ R, G, B });
        float C = M - m; // Chroma

        float H = 0.f; // Hue
        float S = 0.f; // Saturation
        float V = 0.f; // Value

        if (C != 0.f)
        {
            if (M == R)
                H = std::fmod(((G - B) / C), 6.f);
            else if (M == G)
                H = ((B - R) / C) + 2;
            else if (M == B)
                H = ((R - G) / C) + 4;

            H *= 60;
        }

        if (H < 0.f)
            H += 360;

        V = M;
        
        if (V != 0.f)
            S = C / V;

        return { H, S, V };
    }

    // From https://gist.github.com/marukrap/7c361f2c367eaf40537a8715e3fd952a
    sf::Color HSVtoRGB(float H, float S, float V) {
        float C = S * V; // Chroma
        float HPrime = std::fmod(H / 60, 6.f); // H'
        float X = C * (1 - std::fabs(std::fmod(HPrime, 2.f) - 1));
        float M = V - C;

        float R = 0.f;
        float G = 0.f;
        float B = 0.f;

        switch (static_cast<int>(HPrime))
        {
        case 0: R = C; G = X;        break; // [0, 1)
        case 1: R = X; G = C;        break; // [1, 2)
        case 2:        G = C; B = X; break; // [2, 3)
        case 3:        G = X; B = C; break; // [3, 4)
        case 4: R = X;        B = C; break; // [4, 5)
        case 5: R = C;        B = X; break; // [5, 6)
        }

        R += M;
        G += M;
        B += M;

        sf::Color color;
        color.r = static_cast<sf::Uint8>(std::round(R * 255));
        color.g = static_cast<sf::Uint8>(std::round(G * 255));
        color.b = static_cast<sf::Uint8>(std::round(B * 255));

        return color;
    }
}