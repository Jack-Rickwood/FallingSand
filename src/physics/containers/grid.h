#pragma once
#include <vector>
#include <array>
#include <assert.h>
#include <iterator>
#include "cell.h"

template<typename T>
class Grid {
protected:
    std::vector<T> data;

public:
    int width, height;

    Grid(int width_, int height_) :
        width(width_),
        height(height_)
    {
        data.resize(width * height);
    }

    bool checkValidIndex(int index) {
        if (index > (width * height) - 1) {
            return false;
        } else if (index < 0) {
            return false;
        } else {
            return true;
        }
    }

    bool checkValidIndex(sf::Vector2i loc) {
        if (loc.x < 0 || loc.x >= width) {
            return false;
        }
        int index = loc.y * width + loc.x;
        return checkValidIndex(index);
    }

    // Get Element
    T& getElement(int index) {
        return data[index];
    }

    T& getElement(int x, int y) {
        int index = y * width + x;
        return getElement(index);
    }

    T& getElement(sf::Vector2i loc) {
        int index = loc.y * width + loc.x;
        return getElement(index);
    }

    T& operator[](int index) {
        return getElement(index);
    }

    // Set Element
    void setElement(int index, T obj) {
        data[index] = obj;
    }

    void setElement(int x, int y, T obj) {
        int index = y * width + x;
        setElement(index, obj);
    }

    void setElement(sf::Vector2i loc, T obj) {
        int index = loc.y * width + loc.x;
        setElement(index, obj);
    }

    typename std::vector<T>::iterator begin() {
        return data.begin();
    }

    typename std::vector<T>::iterator end() {
        return data.end();
    }

    sf::Vector2i traverse(sf::Vector2i position, sf::Vector2i direction) {
        if (checkValidIndex(position + direction)) {
            return position + direction;
        } else {
            return position;
        }
    }
};

class GameGrid : public Grid<Cell> {
public:
    GameGrid(int width_, int height_) :
        Grid<Cell>(width_, height_)
    {}

    void moveParticle(Cell& cell1, Cell& cell2, sf::Vector2i position_) {
        cell2.setParticle(cell1.particle);
        cell2.particle->setPosition(position_);
        cell1.unsetParticle();
    }

    void swapParticle(Cell& cell1, Cell& cell2) {
        std::shared_ptr<Particle> temp1 = cell1.particle;
        std::shared_ptr<Particle> temp2 = cell2.particle;
        sf::Vector2i pos1 = temp1->getPosition();
        sf::Vector2i pos2 = temp2->getPosition();
        cell2.setParticle(temp1);
        cell2.particle->setPosition(pos2);
        cell1.setParticle(temp2);
        cell1.particle->setPosition(pos1);
    }
};