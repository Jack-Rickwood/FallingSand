#include "../world_generator.h"

void WorldGenerator::spawnTree(GameWorld& world, int x, int surfaceHeight) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> heightDist(80, 120);  // Tree height range
    std::uniform_int_distribution<> widthDist(50, 80);    // Tree width range
    std::uniform_real_distribution<> leafDist(0.0, 1.0);  // For leaf generation

    int treeHeight = heightDist(gen);
    int treeWidth = widthDist(gen);
    int trunkWidth = std::max(5, treeWidth / 10);  // Ensure trunk is at least 3 blocks wide

    // Generate roots
    generateRoots(world, x, surfaceHeight + treeHeight / 10, trunkWidth, gen);

    // Generate trunk
    for (int y = surfaceHeight + treeHeight / 10; y > surfaceHeight - treeHeight; --y) {
        for (int xOffset = -trunkWidth / 2; xOffset <= trunkWidth / 2; ++xOffset) {
            int currentX = x + xOffset;
            if (y >= surfaceHeight - 5 || std::abs(xOffset) == trunkWidth / 2) {
                world.addParticle(GameWorld::createParticle<Bark>(currentX, y));
            } else {
                world.addParticle(GameWorld::createParticle<Wood>(currentX, y));
            }
        }
    }

    // Generate branches and leaves throughout the tree
    int numBranchLevels = 6 + gen() % 4;  // 6 to 9 levels of branches for more variety
    for (int level = 0; level < numBranchLevels; ++level) {
        int branchY = surfaceHeight - treeHeight + (level * treeHeight / numBranchLevels) + (gen() % 10 - 5);  // Add some vertical randomness
        int branchLength = treeWidth / 2 - (level * treeWidth / (3 * numBranchLevels)) + (gen() % 10 - 5);  // Less reduction in length + randomness
        int numBranches = 1 + gen() % 3;  // 1 to 3 branches per level for more variation

        for (int i = 0; i < numBranches; ++i) {
            int branchDir = (gen() % 2 == 0) ? 1 : -1;  // Random direction
            int branchStartX = x + (gen() % (trunkWidth + 1) - trunkWidth / 2) * branchDir;

            for (int j = 0; j < branchLength; ++j) {
                int branchX = branchStartX + branchDir * j;
                int currentY = branchY + j / (4 + gen() % 3);  // Randomize upward angle
                world.addParticle(GameWorld::createParticle<Wood>(branchX, currentY));

                // Add leaves around the branch
                addLeavesAroundPoint(world, branchX, currentY, 3 + gen() % 2, 0.4 + (level / (float)numBranchLevels) * 0.3, gen);

                // Add some smaller branches
                if (j > branchLength / 3 && j % (2 + gen() % 3) == 0) {
                    int smallBranchLength = 2 + gen() % 4;
                    int smallBranchDir = (gen() % 2 == 0) ? 1 : -1;
                    for (int k = 1; k <= smallBranchLength; ++k) {
                        int smallBranchX = branchX + smallBranchDir * k;
                        int smallBranchY = currentY - k / 2;
                        world.addParticle(GameWorld::createParticle<Wood>(smallBranchX, smallBranchY));
                        
                        // Add leaves around the small branch
                        addLeavesAroundPoint(world, smallBranchX, smallBranchY, 2 + gen() % 2, 0.5 + (level / (float)numBranchLevels) * 0.3, gen);
                    }
                }
            }
        }
    }

    // Add extra leaves to fill out the tree shape, with more concentration at the top
    for (int y = surfaceHeight - treeHeight; y > surfaceHeight - treeHeight - treeHeight / 6; --y) {
        float heightFactor = static_cast<float>(surfaceHeight - y) / treeHeight;
        int layerWidth = static_cast<int>(treeWidth * (0.5 + heightFactor * 0.5));  // Wider spread at the top
        for (int xOffset = -layerWidth / 2; xOffset <= layerWidth / 2; ++xOffset) {
            int currentX = x + xOffset;
            
            float distanceFromCenter = static_cast<float>(std::abs(xOffset)) / (layerWidth / 2);
            float leafProbability = std::pow(1.0f - distanceFromCenter, 1.5) * (0.7 + heightFactor * 0.3);  // Higher probability at the top
            
            if (leafDist(gen) < leafProbability * 0.7) {
                world.addParticle(GameWorld::createParticle<Leaf>(currentX, y));
            }
        }
    }

    // Add a dense cluster of leaves at the very top
    addLeavesAroundPoint(world, x, surfaceHeight - treeHeight - treeHeight / 6, treeWidth / 4, 0.8, gen);
}

void WorldGenerator::generateRoots(GameWorld& world, int x, int surfaceHeight, int trunkWidth, std::mt19937& gen) {
    std::uniform_int_distribution<> rootCountDist(3, 5);  // Number of main roots
    std::uniform_int_distribution<> rootLengthDist(10, 20);  // Length of main roots
    std::uniform_int_distribution<> rootAngleDist(20, 70);  // Angle of roots in degrees

    int numRoots = rootCountDist(gen);

    for (int i = 0; i < numRoots; ++i) {
        int rootLength = rootLengthDist(gen);
        int rootAngle = rootAngleDist(gen);
        
        // Alternate between left and right sides
        bool isRightSide = (i % 2 == 0);
        if (!isRightSide) {
            rootAngle = 180 - rootAngle;  // Mirror the angle for the left side
        }
        
        float angleRad = rootAngle * M_PI / 180.0f;
        float cosAngle = std::cos(angleRad);
        float sinAngle = std::sin(angleRad);

        int startX = x + (isRightSide ? trunkWidth/2 : -trunkWidth/2);
        int startY = surfaceHeight;

        for (int j = 0; j < rootLength; ++j) {
            int rootX = startX + static_cast<int>(j * cosAngle);
            int rootY = startY + static_cast<int>(j * sinAngle);

            // Add some randomness to root shape
            rootX += (gen() % 3) - 1;
            rootY += (gen() % 3) - 1;

            world.addParticle(GameWorld::createParticle<Wood>(rootX, rootY));

            // Add smaller root offshoots
            if (j > rootLength / 2 && j % 3 == 0) {
                int offshootLength = 2 + gen() % 3;
                int offshootAngle = rootAngle + (gen() % 60 - 30);
                float offshootAngleRad = offshootAngle * M_PI / 180.0f;

                for (int k = 1; k <= offshootLength; ++k) {
                    int offshootX = rootX + static_cast<int>(k * std::cos(offshootAngleRad));
                    int offshootY = rootY + static_cast<int>(k * std::sin(offshootAngleRad));
                    world.addParticle(GameWorld::createParticle<Wood>(offshootX, offshootY));
                }
            }
        }
    }
}

void WorldGenerator::addLeavesAroundPoint(GameWorld& world, int centerX, int centerY, int radius, float probability, std::mt19937& gen) {
    std::uniform_real_distribution<> leafDist(0.0, 1.0);
    for (int leafX = centerX - radius; leafX <= centerX + radius; ++leafX) {
        for (int leafY = centerY - radius; leafY <= centerY + radius; ++leafY) {
            float distance = std::sqrt(std::pow(leafX - centerX, 2) + std::pow(leafY - centerY, 2));
            if (distance <= radius && leafDist(gen) < probability * (1 - distance / radius)) {
                world.addParticle(GameWorld::createParticle<Leaf>(leafX, leafY));
            }
        }
    }
}