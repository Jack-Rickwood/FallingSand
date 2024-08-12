#include "rng.h"

typedef std::mt19937 rng_type;

int Rand::range(int min, int max) {
    static std::minstd_rand gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

float Rand::range(float min, float max) {
    static std::minstd_rand gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int Rand::range(int min, int max, unsigned int seed) {
    int range = max - min + 1;
    return min + (seed % range);
}