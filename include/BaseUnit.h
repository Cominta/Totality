#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>
#include "tilemap.h"
#include <queue>

struct TaskMove
{
    std::vector<sf::RectangleShape> path;
    sf::Vector2f wayEnd;
};

class BaseUnit
{
private:
    sf::CircleShape unit;
    // sf::Vector2i wayEnd;
    sf::RenderWindow* window;
    Tilemap* tilemap;
    bool b_active;
    bool b_moving;

    const int speed;
    int currentSpeed;

    int xMap;
    int yMap;

    // std::vector<sf::RectangleShape> path;
    std::queue<TaskMove> tasks;
    std::vector<sf::RectangleShape> predictPath(std::vector<std::vector<int>>& map, sf::Vector2f wayEnd, float& startX, float& startY, bool& success);
    void clearTasks();

public:
    BaseUnit(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits)
        : speed(50)
    {
        unit.setRadius(32.f);
        unit.setOrigin(32.f, 32.f);
        unit.setFillColor(sf::Color(100, 100, 100));
        unit.setOutlineThickness(2.5f);
        unit.setOutlineColor(sf::Color(0, 0, 0));
        unit.setPosition(xMap * 64 + 32, yMap * 64 + 32);
        b_active = false;
        b_moving = false;
        this->currentSpeed = 0;
        window = _window;
        this->tilemap = tilemap;
        mapUnits[yMap][xMap] = 1;
        this->xMap = xMap;
        this->yMap = yMap;

        // this->wayEnd.x = this->unit.getPosition().x;
        // this->wayEnd.y = this->unit.getPosition().y;
    }


    ~BaseUnit()
    {}

    void moveTo(std::vector<std::vector<int>>& mapUnits);

    void setActive(bool _active)
    {
        b_active = _active;
    }
    bool isActiv() const
    {
        return b_active;
    }

    void setIsMoving(bool status)
    {
        b_moving = status;
    }
    bool isMoving() const
    {
        return b_moving;
    }

    void setOutlineThickness(float wide)
    {
        unit.setOutlineThickness(wide);
    }
    void setOutlineColor(sf::Uint8 R, sf::Uint8 G, sf::Uint8 B)
    {
        unit.setOutlineColor(sf::Color(R, G, B));
    }
    void setFillColor(sf::Uint8 R, sf::Uint8 G, sf::Uint8 B)
    {
        unit.setFillColor(sf::Color(R, G, B));
    }
    void setRadius(float radius)
    {
        unit.setRadius(radius);
    }
    bool GlobalBoundContainCheck(sf::Vector2i cord)
    {
        return unit.getGlobalBounds().contains(sf::Vector2f(cord));
    }
    void move(float X, float Y)
    {
        unit.move(X, Y);
    }

    void setPosition(int x, int y, std::vector<std::vector<int>>& vector)
    {
        vector[y][x] = 1;
        this->xMap = x;
        this->yMap = y;
    }

    void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, std::vector<std::vector<int>>& mapUnits, std::vector<std::vector<int>>& map);
    bool find(std::vector<int> keys, int item)
    {
        for (int i = 0; i < keys.size(); i++)
        {
            if (keys[i] == item)
            {
                return true;
            }
        }

        return false;
    }

    void render();
};
