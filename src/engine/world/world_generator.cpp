#include "world_generator.h"
#include <random>
#include <algorithm>
#include <cmath>

void WorldGenerator::generate(GameWorld& world) {
    int width = world.getSize().x;
    int height = world.getSize().y;

    // Generate base terrain
    std::vector<int> surfaceHeights(width);
    for (int x = 0; x < width; x++) {
        Biome biome = getBiome(x, 0, 0, false);
        float terrainValue = terrainNoise.GetNoise(static_cast<float>(x), 0.0f);
        float hilliness = getBiomeHilliness(biome, x);
        float elevation = getBiomeElevation(biome, x);
        surfaceHeights[x] = static_cast<int>((terrainValue * hilliness + 0.5f) * 500.0f + height * 0.3f + elevation);
    }

    // Smooth terrain
    for (int i = 0; i < 4; i++) {
        std::vector<int> smoothedHeights = surfaceHeights;
        for (int x = 1; x < width - 1; x++) {
            smoothedHeights[x] = (surfaceHeights[x-1] + surfaceHeights[x] + surfaceHeights[x+1]) / 3;
        }
        surfaceHeights = smoothedHeights;
    }

    // Generate terrain
    for (int x = 0; x < width; x++) {
        int surfaceHeight = surfaceHeights[x];

        for (int y = 0; y < height; y++) {
            std::shared_ptr<Particle> particle = getParticleAtLocation(
                world, x, y,
                surfaceHeight
            );
            if (particle) {
                world.addParticle(particle);
            }
        }

        gen_progress = static_cast<int>(100.0f * (x + 1) / width);
    }

    for (int x = 0; x < width; x++) {
        int surfaceHeight = surfaceHeights[x];

        // Spawn trees in plains
        if (getBiome(x, surfaceHeight, surfaceHeight, false) == Biome::Plains) {
            float objectValue = dis(gen);
            if (objectValue < 0.01f) {
                spawnTree(world, x, surfaceHeight);
            }
        }
    }
    gen_progress = 100;
}

std::shared_ptr<Particle> WorldGenerator::getParticleAtLocation(GameWorld& world, int x, int y, int surfaceHeight) {
    int baseDirtLayerThickness = 200;
    int caveZonePadding = 100;
    Biome biome = getBiome(x, y, surfaceHeight);

    switch (biome) {
        case Biome::Desert: {
            if (y < surfaceHeight) {
                return nullptr;
            } else {
                return GameWorld::createParticle<Sand>(x, y);
            }
        }
        case Biome::Mountains: {
            if (y < surfaceHeight) {
                return nullptr;
            } else if (y < surfaceHeight + caveZonePadding) {
                return GameWorld::createParticle<Stone>(x, y);
            } else {
                return generateUndergroundParticle(biome, x, y, caveZonePadding, surfaceHeight);
            }
        }
        case Biome::Ocean: {
            int oceanDepth = 500;
            if (y < surfaceHeight) {
                return nullptr;
            } else if (y < surfaceHeight + oceanDepth) {
                return GameWorld::createParticle<Water>(x, y);
            } else {
                return GameWorld::createParticle<Sand>(x, y);
            }
        }
        case Biome::Plains: {
            float dirtBlendFactor = biomeBoundaryNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
            int dirtLayerThickness = baseDirtLayerThickness + static_cast<int>(dirtBlendFactor * 30);
            int noiseThickness = 15;
            int y_noisy = y + (dis(gen) - 0.5f) * 2.0f * noiseThickness;
            if (y < surfaceHeight) {
                return nullptr;
            } else if (y < surfaceHeight + 3) {
                return GameWorld::createParticle<Grass>(x, y);
            } else if (y < surfaceHeight + dirtLayerThickness - noiseThickness) {
                return GameWorld::createParticle<Dirt>(x, y);
            } else {
                if (y_noisy < surfaceHeight + dirtLayerThickness) {
                    return GameWorld::createParticle<Dirt>(x, y);
                } else if (y_noisy < surfaceHeight + dirtLayerThickness + caveZonePadding) {
                    return GameWorld::createParticle<Stone>(x, y);
                } else {
                    return generateUndergroundParticle(biome, x, y, caveZonePadding, surfaceHeight);
                }
            }
        }
        case Biome::Volcanic: {
            if (y < surfaceHeight) {
                return nullptr;
            } else if (y < surfaceHeight + caveZonePadding) {
                return GameWorld::createParticle<VolcanicRock>(x, y);
            } else {
                float lavaValue = lakeNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
                if (lavaValue > 0.7f && y < surfaceHeight + 50) {
                    return GameWorld::createParticle<Lava>(x, y);
                } else {
                    return generateUndergroundParticle(biome, x, y, caveZonePadding, surfaceHeight);
                }
            }
        }
        default: {
            return nullptr;
        }
    }
}

std::shared_ptr<Particle> WorldGenerator::generateUndergroundParticle(Biome biome, int x, int y, int caveZonePadding, int surfaceHeight) {
    float caveValue = caveNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float ironOreValue = ironOreNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float fetteriteOreValue = fetteriteOreNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
    float altStoneValue = altStoneNoise.GetNoise(static_cast<float>(x), static_cast<float>(y));
    
    if (caveValue > 0.3f && biome == getBiome(x + caveZonePadding, y, surfaceHeight, false) && biome == getBiome(x - caveZonePadding, y, surfaceHeight, false)) {
        return nullptr;
    } else if (ironOreValue > 0.8f) {
        return GameWorld::createParticle<Iron>(x, y);
    } else if (fetteriteOreValue > 0.92f) {
        return GameWorld::createParticle<Fetterite>(x, y);
    } else if (altStoneValue > 0.4f) {
        if (biome == Biome::Volcanic) {
            return GameWorld::createParticle<Obsidian>(x, y);
        } else {
            return nullptr;
        }
    } else {
        if (biome == Biome::Volcanic) {
            return GameWorld::createParticle<VolcanicRock>(x, y);
        } else {
            return GameWorld::createParticle<Stone>(x, y);
        }
    }
}

Biome WorldGenerator::getBiome(int x, int y, int surfaceHeight, bool blending) {
    float biomeValue = biomeNoise.GetNoise(static_cast<float>(x) * biomeSize, 0.0f);

    if (y > surfaceHeight) {
        biomeValue += biomeBoundaryNoise.GetNoise(static_cast<float>(x), static_cast<float>(y)) * 0.1f * biomeSize;
    }

    if (blending && y > surfaceHeight) {
        biomeValue += (dis(gen) - 0.5f) * 0.05f * biomeSize;
    }

    if (biomeValue < -0.5f) return Biome::Ocean;
    if (biomeValue < -0.2f) return Biome::Desert;
    if (biomeValue > 0.6f) return Biome::Volcanic;
    if (biomeValue > 0.2f) return Biome::Mountains;
    return Biome::Plains;
}

float WorldGenerator::getBiomeHilliness(Biome biome, int x) {
    float baseHilliness;
    switch (biome) {
        case Biome::Desert: baseHilliness = 0.25f; break;
        case Biome::Mountains: baseHilliness = 0.7f; break;
        case Biome::Ocean: baseHilliness = 0.01f; break;
        case Biome::Plains: baseHilliness = 0.3f; break;
        case Biome::Volcanic: baseHilliness = 0.5f; break;
        default: baseHilliness = 0.5f;
    }

    float biomeValue = biomeNoise.GetNoise(static_cast<float>(x) * biomeSize, 0.0f);
    float transitionWidth = 0.1f;
    
    float distanceFromBoundary = std::min({
        std::abs(biomeValue + 0.5f), // Ocean boundary
        std::abs(biomeValue + 0.2f), // Desert boundary
        std::abs(biomeValue - 0.2f), // Mountains lower boundary
        std::abs(biomeValue - 0.6f)  // Volcanic lower boundary
    });
    
    float transitionFactor = glm::clamp((distanceFromBoundary / transitionWidth), 0.0f, 1.0f);
    transitionFactor = transitionFactor * transitionFactor * (3 - 2 * transitionFactor); // Smoothstep function
    
    return baseHilliness * transitionFactor + 0.2f * (1 - transitionFactor);
}

int WorldGenerator::getBiomeElevation(Biome biome, int x) {
    float biomeValue = biomeNoise.GetNoise(static_cast<float>(x) * biomeSize, 0.0f);
    
    // Define base elevations for each biome
    std::unordered_map<Biome, int> baseElevations = {
        {Biome::Ocean, 100},
        {Biome::Desert, 0},
        {Biome::Plains, 0},
        {Biome::Mountains, -250},
        {Biome::Volcanic, -150}
    };

    // Define biome boundaries
    std::vector<std::pair<float, Biome>> biomeBoundaries = {
        {-0.5f, Biome::Ocean},
        {-0.2f, Biome::Desert},
        {0.2f, Biome::Plains},
        {0.6f, Biome::Mountains},
        {1.0f, Biome::Volcanic}
    };

    // Find the two nearest biomes
    Biome lowerBiome = Biome::Plains;
    Biome upperBiome = Biome::Plains;
    float lowerBoundary = -1.0f;
    float upperBoundary = 1.0f;

    for (size_t i = 0; i < biomeBoundaries.size(); ++i) {
        if (biomeValue <= biomeBoundaries[i].first) {
            if (i > 0) {
                lowerBiome = biomeBoundaries[i-1].second;
                lowerBoundary = biomeBoundaries[i-1].first;
            }
            upperBiome = biomeBoundaries[i].second;
            upperBoundary = biomeBoundaries[i].first;
            break;
        }
    }

    // Calculate blend factor
    float blendFactor = (biomeValue - lowerBoundary) / (upperBoundary - lowerBoundary);
    blendFactor = glm::clamp(blendFactor, 0.0f, 1.0f);

    // Smoothstep function for more natural transitions
    blendFactor = blendFactor * blendFactor * (3 - 2 * blendFactor);

    // Interpolate between the two nearest biome elevations
    int lowerElevation = baseElevations[lowerBiome];
    int upperElevation = baseElevations[upperBiome];
    
    return static_cast<int>(lowerElevation + blendFactor * (upperElevation - lowerElevation));
}