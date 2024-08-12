#pragma once
#include <random>
#include <limits>

namespace Rand {
    int range(int min, int max);
    float range(float min, float max);
    int range(int min, int max, unsigned int seed);
}