#pragma once
#include <thread>
#include <atomic>
#include "glm/glm.hpp"
#include "game_world.h"
#include <externals/FastNoiseLite/FastNoiseLite.h>
#include <unordered_map>

enum class Biome {
    Plains,
    Desert,
    Mountains,
    Ocean,
    Volcanic
};

class WorldGenerator {
private:
    FastNoiseLite terrainNoise;
    FastNoiseLite dirtBlendNoise;
    FastNoiseLite caveNoise;
    FastNoiseLite ironOreNoise;
    FastNoiseLite fetteriteOreNoise;
    FastNoiseLite biomeNoise;
    FastNoiseLite biomeBoundaryNoise;
    FastNoiseLite altStoneNoise;
    FastNoiseLite lakeNoise;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    std::atomic<int> gen_progress;

    float biomeSize;

public:
    WorldGenerator(int seed, float biomeSize_) :
        terrainNoise(seed),
        dirtBlendNoise(seed),
        caveNoise(seed),
        ironOreNoise(seed),
        fetteriteOreNoise(seed),
        biomeNoise(seed),
        biomeBoundaryNoise(seed),
        altStoneNoise(seed),
        lakeNoise(seed),
        gen(seed),
        dis(0.0, 1.0),
        gen_progress(0),
        biomeSize(biomeSize_)
    {
        terrainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        terrainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        terrainNoise.SetFractalOctaves(4);
        terrainNoise.SetFrequency(0.003f);

        caveNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        caveNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        caveNoise.SetFractalOctaves(4);
        caveNoise.SetFrequency(0.008f);

        ironOreNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        ironOreNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        ironOreNoise.SetFractalOctaves(2);
        ironOreNoise.SetFrequency(0.015f);

        fetteriteOreNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        fetteriteOreNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        fetteriteOreNoise.SetFractalOctaves(2);
        fetteriteOreNoise.SetFrequency(0.02f);

        biomeNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        biomeNoise.SetFractalOctaves(2);
        biomeNoise.SetFrequency(0.001f);

        biomeBoundaryNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        biomeBoundaryNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        biomeBoundaryNoise.SetFractalOctaves(4);
        biomeBoundaryNoise.SetFrequency(0.008f);

        altStoneNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        altStoneNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        altStoneNoise.SetFractalOctaves(4);
        altStoneNoise.SetFrequency(0.008f);

        lakeNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        lakeNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
        lakeNoise.SetFractalOctaves(4);
        lakeNoise.SetFrequency(0.003f);
    }

    Biome getBiome(int x, int y, int surfaceHeight, bool blending = true);
    void generate(GameWorld& world);
    std::shared_ptr<Particle> getParticleAtLocation(GameWorld& world, int x, int y, int surfaceHeight);
    std::shared_ptr<Particle> generateUndergroundParticle(Biome biome, int x, int y, int caveZonePadding, int surfaceHeight);

    std::atomic<int>& getGenProgress() { return gen_progress; }

    float getMountainBlendFactor(int x);
    float getBiomeHilliness(Biome biome, int x);
    int getBiomeElevation(Biome biome, int x);
    void spawnTree(GameWorld& world, int x, int surfaceHeight);
    void generateRoots(GameWorld& world, int x, int surfaceHeight, int trunkWidth, std::mt19937& gen);
    void addLeavesAroundPoint(GameWorld& world, int centerX, int centerY, int radius, float probability, std::mt19937& gen);
    float getVolcanicBlendFactor(int x);
};