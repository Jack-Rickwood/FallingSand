#pragma once
#include <SFML/Graphics.hpp>
#include "../graphics/color.h"
#include "utils/rng.h"
#include <stdint.h>

class GameGrid;

enum class ParticleForm {
    UNK,
    SOLID,
    LIQUID,
    GAS
};

/*===== Base Particle =====*/
class Particle {
protected:
    sf::Vector2i position;
    sf::Vector2i velocity;
    sf::Vector2i acceleration;
    sf::Color color;

    float temperature = 50.0f;

    bool stuck = false;
    bool particle_mode = false;

public:
    Particle(sf::Vector2i position_, sf::Color color_) {
        position = position_;
        velocity = {0, 0};
        acceleration = {0, 0};
        color = color_;
    }
    Particle() {
        Particle({0, 0}, sf::Color::White);
    }

    void clear() {
        position = {0, 0};
        velocity = {0, 0};
        acceleration = {0, 0};
        color = sf::Color::White;
    }

    virtual sf::Color getColor() {
        return color;
    }

    virtual ParticleForm getForm() {
        return ParticleForm::UNK;
    }

    virtual sf::Vector2i getGravity() {
        return {0, 1};
    }

    virtual float getDensity() {
        return 1.0f;
    }

    virtual float getResistance() {
        return 1.0f;
    }

    virtual float getFlammability() {
        return 0.0f;
    }

    virtual bool canBurn() {
        return false;
    }

    virtual bool enableBloom() {
        return false;
    }

    virtual void update(GameGrid& state);
    virtual void particleInteractions(GameGrid& state);

    std::vector<sf::Vector2i> getPath(sf::Vector2i pos, sf::Vector2i move);
    virtual bool applyVelocity(GameGrid& state);
    float calculateTempTransfer(float temp_curr, float temp_targ, float flam, float prox_adj, int upd_per_sec);

    sf::Vector2i getPosition() {
        return position;
    }

    void setPosition(sf::Vector2i pos) {
        position = pos;
    }

    sf::Vector2i getVelocity() {
        return velocity;
    }

    void setVelocity(sf::Vector2i vel) {
        velocity = vel;
    }

    sf::Vector2i getAcceleration() {
        return acceleration;
    }

    void setAcceleration(sf::Vector2i accel) {
        acceleration = accel;
    }

    float getTemperature() {
        return temperature;
    }

    void setTemperature(float temp) {
        temperature = temp;
    }
};


/*===== Forms =====*/
// Solids
class Solid : public Particle {
public:
    Solid() = default;
    Solid(sf::Vector2i position_, sf::Color color_) :
        Particle(position_, color_)
    {}

    virtual ParticleForm getForm() {
        return ParticleForm::SOLID;
    }
};

class MovableSolid : public Solid {
public:
    MovableSolid() = default;
    MovableSolid(sf::Vector2i position_, sf::Color color_) :
        Solid(position_, color_)
    {}

    virtual void update(GameGrid& state);
};

class ImmovableSolid : public Solid {
public:
    ImmovableSolid() = default;
    ImmovableSolid(sf::Vector2i position_, sf::Color color_) :
        Solid(position_, color_)
    {}

    virtual void update(GameGrid& state);
};

// Liquid
class Liquid : public Particle {
public:
    Liquid() = default;
    Liquid(sf::Vector2i position_, sf::Color color_) :
        Particle(position_, color_)
    {}

    virtual ParticleForm getForm() {
        return ParticleForm::LIQUID;
    }

    virtual void update(GameGrid& state);

    virtual void disperse(GameGrid& state, sf::Vector2i dir);
};

// Gas
class Gas : public Particle {
public:
    Gas() = default;
    Gas(sf::Vector2i position_, sf::Color color_) :
        Particle(position_, color_)
    {}

    virtual ParticleForm getForm() {
        return ParticleForm::GAS;
    }

    virtual sf::Vector2i getGravity() {
        return {0, -1};
    }

    virtual void update(GameGrid& state);

    virtual void disperse(GameGrid& state, sf::Vector2i dir);
};


/*===== Types =====*/
class Sand : public MovableSolid {
private:
    Sand(sf::Vector2i position_, int color_mod) :
        MovableSolid(position_, sf::Color(232 + color_mod, 199 + color_mod, 97 + color_mod))
    {}

public:
    Sand() = default;
    Sand(sf::Vector2i position_) :
        Sand(position_, Rand::range(-2, 2) * 10)
    {}

    float getResistance() {
        return 0.0f;
    }
};

class Ember : public MovableSolid {
private:
    Ember(sf::Vector2i position_, std::tuple<float, float, float> hsv_col) :
        MovableSolid(position_, PColor::HSVtoRGB(std::get<0>(hsv_col) + (float)Rand::range(-2, 1) * 8.0f,
                                                 std::get<1>(hsv_col) + 0.1f,
                                                 std::get<2>(hsv_col)))
    {}

public:
    Ember() = default;
    Ember(sf::Vector2i position_) :
        Ember(position_, PColor::RGBtoHSV(sf::Color(190, 105, 20)))
    {
        temperature = 100.0f;
    }

    float getResistance() {
        return 0.0f;
    }

    bool enableBloom() {
        return true;
    }

    void particleInteractions(GameGrid& state);
};

class Dirt : public MovableSolid {
private:
    Dirt(sf::Vector2i position_, int color_mod) :
        MovableSolid(position_, sf::Color(85 + color_mod, 48 + color_mod, 13 + color_mod))
    {}

public:
    Dirt() = default;
    Dirt(sf::Vector2i position_) :
        Dirt(position_, Rand::range(-2, 2) * 4)
    {}

    float getResistance() {
        return 0.1f;
    }
};

class Grass : public MovableSolid {
private:
    Grass(sf::Vector2i position_, int color_mod) :
        MovableSolid(position_, sf::Color(91 + color_mod, 135 + color_mod, 49 + color_mod))
    {}

public:
    Grass() = default;
    Grass(sf::Vector2i position_) :
        Grass(position_, Rand::range(-2, 2) * 3)
    {}

    float getResistance() {
        return 0.1f;
    }
};

class Stone : public ImmovableSolid {
private:
    Stone(sf::Vector2i position_, int color_mod) :
        ImmovableSolid(position_, sf::Color(175 + color_mod, 164 + color_mod, 151 + color_mod))
    {}

public:
    Stone() = default;
    Stone(sf::Vector2i position_) :
        Stone(position_, Rand::range(-2, 2) * 5)
    {}

    float getResistance() {
        return 0.8f;
    }
};

class VolcanicRock : public ImmovableSolid {
private:
    VolcanicRock(sf::Vector2i position_, int color_mod) :
        ImmovableSolid(position_, sf::Color(55, 20 + color_mod, 20 + color_mod))
    {}

public:
    VolcanicRock() = default;
    VolcanicRock(sf::Vector2i position_) :
        VolcanicRock(position_, Rand::range(-2, 2) * 5)
    {}

    float getResistance() {
        return 0.8f;
    }
};

class Obsidian : public ImmovableSolid {
private:
    Obsidian(sf::Vector2i position_, int color_mod) :
        ImmovableSolid(position_, color_mod ? sf::Color(46, 41, 58) : sf::Color(46, 49, 52))
    {}

public:
    Obsidian() = default;
    Obsidian(sf::Vector2i position_) :
        Obsidian(position_, Rand::range(0, 1))
    {}

    float getResistance() {
        return 0.8f;
    }
};

class Iron : public ImmovableSolid {
public:
    Iron() = default;
    Iron(sf::Vector2i position_) :
        ImmovableSolid(position_, sf::Color(203, 205, 205))
    {}

    float getResistance() {
        return 0.8f;
    }
};

class Fetterite : public ImmovableSolid {
public:
    Fetterite() = default;
    Fetterite(sf::Vector2i position_) :
        ImmovableSolid(position_, sf::Color(0, 156, 74))
    {}

    float getResistance() {
        return 0.8f;
    }
};

class Wood : public ImmovableSolid {
private:
    Wood(sf::Vector2i position_, int color_mod) :
        ImmovableSolid(position_, sf::Color(70 - 1.5 * color_mod, 58 - color_mod, 34 - color_mod/2))
    {}

public:
    Wood() = default;
    Wood(sf::Vector2i position_) :
        Wood(position_, ((position_.x % 4 == 0) ? Rand::range(0, 3) : Rand::range(0, 18)) < 3 ? 10 : 0)
    {}

    float getResistance() {
        return 0.8f;
    }

    float getFlammability() {
        return 0.75f;
    }

    void particleInteractions(GameGrid& state);
};

class Bark : public ImmovableSolid {
public:
    Bark() = default;
    Bark(sf::Vector2i position_) :
        ImmovableSolid(position_, sf::Color(70, 58, 34))
    {}

    float getResistance() {
        return 0.8f;
    }

    float getFlammability() {
        return 0.75f;
    }
};

class Leaf : public ImmovableSolid {
private:
    Leaf(sf::Vector2i position_, int color_mod) :
        ImmovableSolid(position_, sf::Color(88 + color_mod, 126 + color_mod, 96 + color_mod))
    {}

public:
    Leaf() = default;
    Leaf(sf::Vector2i position_) :
        Leaf(position_, Rand::range(-2, 2) * 8)
    {}

    float getResistance() {
        return 0.8f;
    }

    float getFlammability() {
        return 0.75f;
    }
};

class Fire : public ImmovableSolid {
private:
    int extinguish_ct = 0;

public:
    Fire() = default;
    Fire(sf::Vector2i position_) :
        ImmovableSolid(position_, sf::Color(182, 34, 3))
    {
        temperature = 100.0f;
    }

    float getResistance() {
        return 0.0f;
    }

    bool enableBloom() {
        return true;
    }

    void particleInteractions(GameGrid& state);
};

class Water : public Liquid {
public:
    Water() = default;
    Water(sf::Vector2i position_) :
        Liquid(position_, sf::Color(14, 135, 204))
    {}

    float getDensity() {
        return 0.5f;
    }

    float getResistance() {
        return 0.25f;
    }
};

class Lava : public Liquid {
public:
    Lava() = default;
    Lava(sf::Vector2i position_) :
        Liquid(position_, sf::Color(207, 16, 32))
    {}

    float getDensity() {
        return 0.5f;
    }

    float getResistance() {
        return 0.25f;
    }

    bool enableBloom() {
        return true;
    }
};

class Acid : public Liquid {
public:
    Acid() = default;
    Acid(sf::Vector2i position_) :
        Liquid(position_, sf::Color(120, 219, 0))
    {}

    float getDensity() {
        return 0.5f;
    }

    float getResistance() {
        return 0.25f;
    }

    float getCorrosivity() {
        return 0.2f;
    }

    bool enableBloom() {
        return true;
    }

    void particleInteractions(GameGrid& state);
};

class Smoke : public Gas {
public:
    Smoke() = default;
    Smoke(sf::Vector2i position_) :
        Gas(position_, sf::Color(115, 130, 118, 150))
    {}

    float getDensity() {
        return 0.2f;
    }

    float getResistance() {
        return 0.0f;
    }
};