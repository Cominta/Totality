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
protected:
    sf::CircleShape unit;
    // sf::Vector2i wayEnd;
    sf::RenderWindow* window;
    Tilemap* tilemap;
    bool b_active;
    bool b_moving;

    bool attack;
    BaseUnit* toAttack;

    const int speed;
    const int speedAttack;
    int currentSpeed;
    int currentSpeedAttack;

    int hp;
    const int maxHp;
    int damage;
    bool attacked;

    int xMap;
    int yMap;
    bool slowed;

    sf::RectangleShape hpBar;
    sf::RectangleShape hpBarBack;

    // std::vector<sf::RectangleShape> path;
    std::queue<TaskMove> tasks;
    std::vector<sf::RectangleShape> predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success);
    void clearTasks();
    void initHpBar();
    void updateHpBar();

public:
    BaseUnit(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits)
        : speed(70), speedAttack(20), maxHp(100)
    {
        this->slowed = false;
        this->hp = 100;
        this->attacked = false;
        this->damage = 10;
        unit.setRadius(32.f);
        unit.setOrigin(32.f, 32.f);
        unit.setFillColor(sf::Color(100, 100, 100));
        unit.setOutlineThickness(2.5f);
        unit.setOutlineColor(sf::Color(0, 0, 0));
        unit.setPosition(xMap * 64 + 32, yMap * 64 + 32);
        b_active = false;
        b_moving = false;
        this->currentSpeed = 0;
        this->currentSpeedAttack = 0;
        window = _window;
        this->tilemap = tilemap;
        mapUnits[yMap][xMap] = 1;
        this->xMap = xMap;
        this->yMap = yMap;

        this->initHpBar();

        // this->wayEnd.x = this->unit.getPosition().x;
        // this->wayEnd.y = this->unit.getPosition().y;
    }


    ~BaseUnit()
    {
        toAttack = nullptr;
    }

    void moveTo();
    int getHp() {return this->hp;}

    void setActive(bool _active)
    {
        this->b_active = _active;

        if (this->b_active)
        {
            this->setOutlineColor(255, 0 ,0);
        }

        else 
        {
            this->setOutlineColor(0, 0 ,0);
        }
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
        unit.move(X / 64, Y / 64);
    }

    void doDamage(int damage)
    {
        this->hp -= damage;
        this->attacked = true;
    }

    bool getAttacked() {return this->attacked;}

    void setPosition(int x, int y, std::vector<std::vector<int>>& vector)
    {
        vector[y][x] = 1;
        this->xMap = x;
        this->yMap = y;
    }

    int getX() {return this->xMap;}
    int getY() {return this->yMap;}

    void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& realisedKeys, std::vector<int>& pressedKeys, std::vector<BaseUnit*>& units);
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

    sf::CircleShape getShape() {return this->unit;}

    void renderGame(sf::View view);
    void renderMini(sf::View view);
};
