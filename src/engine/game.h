#include "world/game_world.h"
#include "world/world_generator.h"
#include "../graphics/renderer.h"
#include "../input/input_handler.h"
#include "player.h"

class Game {
public:
    enum GameMode {
        SURVIVAL,
        CREATIVE,
        MAP_PREVIEW
    };

    Game(int win_width_, int win_height_, int framerate_, int world_width_, int world_height_, float render_scale_, GameMode mode_, int seed, float biome_size) :
        world({world_width_, world_height_}),
        world_generator(seed, biome_size),
        renderer(win_width_, win_height_, framerate_, render_scale_),
        mode(mode_),
        world_width(world_width_),
        world_height(world_height_)
    {
        player.setPos(sf::Vector2i(world_width_/2.0f, world_height_/2));
        camera_pos = player.getPos();
    }

    void start();
    void loop(bool evolve_world);

private:
    // Initialised in this order, regardless of order in initialization list
    GameWorld world;
    WorldGenerator world_generator;
    Player player;
    Renderer renderer;
    GameMode mode;
    InputHandler input_handler;

    sf::Vector2i camera_pos;
    int world_width;
    int world_height;

    sf::Clock clock;
    int frame_num = 0;
    float frame_time = 0;
    float fps = 0;
};