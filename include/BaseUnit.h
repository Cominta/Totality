#pragma once

#include "SFML\Graphics.hpp"
#include <iostream>
#include "tilemap.h"
#include <queue>
#include "team.h"
#include "AStar.hpp"
#include "sounds.h"
#include "randomNums.h"

struct TaskMove
{
    std::vector<sf::RectangleShape> path;
    sf::Vector2f wayEnd;
};

class BaseUnit
{
protected:
    friend class Archer;
    friend class Warrior;

    sf::Sprite unit;
    sf::Texture normalTexture;
    sf::Color normalColor;

    sf::Texture activeTexture;
    sf::Color activeColor;

    // sf::Vector2i wayEnd;
    sf::RenderWindow* window;
    Tilemap* tilemap;
    bool b_active;
    bool b_moving;

    bool attack;
    BaseUnit* toAttack;

    int speed;
    int speedAttack;
    int currentSpeed;
    int currentSpeedAttack;

    int hp;
    int maxHp;
    int damage;
    bool attacked;

    int xMap;
    int yMap;
    bool slowed;
    AStar::Generator generator;

    sf::RectangleShape hpBar;
    sf::RectangleShape hpBarBack;

    // std::vector<sf::RectangleShape> path;
    std::queue<TaskMove> tasks;
    virtual std::vector<sf::RectangleShape> predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success);
    bool newPredict();
    void clearTasks();
    void initHpBar();
    sf::Texture editSprite(sf::Color color, sf::Texture texture);
    void flipTexture(int newX, int newY);

    Team team;

public:
    BaseUnit(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits, Team _team, sf::Texture texture)
        : speed(6), speedAttack(20), maxHp(100)
    {
        this->slowed = false;
        this->hp = 100;
        this->attacked = false;
        this->damage = 10;
        team = _team;

        if (team == 0)
        {
            this->normalColor = sf::Color(255, 0, 0);
            this->activeColor = sf::Color(100, 0, 0);
        }

        else if (team == 1)
        {
            this->normalColor = sf::Color(0, 0, 255);
            this->activeColor = sf::Color(0, 0, 100);
        }

        else if (team == 2)
        {
            this->normalColor = sf::Color(0, 255, 0);
            this->activeColor = sf::Color(0, 100, 0);
        }

        else if (team == 3)
        {
            this->normalColor = sf::Color(255, 255, 0);
            this->activeColor = sf::Color(100, 100, 0);
        }

        this->normalTexture = this->editSprite(this->normalColor, texture);
        this->activeTexture = this->editSprite(this->activeColor, texture);

        this->unit.setOrigin(this->normalTexture.getSize().x / 2, this->normalTexture.getSize().y / 2);
        this->unit.setTexture(this->normalTexture);

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
        this->toAttack = nullptr;

        this->generator.setWorldSize({this->tilemap->getWidth(), this->tilemap->getHeight()});
        this->generator.setHeuristic(AStar::Heuristic::euclidean);
        this->generator.setDiagonalMovement(true);

        for (int y = 0; y < this->tilemap->getHeight(); y++)
        {
            for (int x = 0; x < this->tilemap->getWidth(); x++)
            {
                if (this->tilemap->map[y][x] < this->tilemap->tileKeys["sand"].first || this->tilemap->map[y][x] > this->tilemap->tileKeys["ground"].second)
                {
                    this->generator.addCollision({x, y});
                }
            }
        }
        // this->wayEnd.x = this->unit.getPosition().x;
        // this->wayEnd.y = this->unit.getPosition().y;
    }


    ~BaseUnit()
    {
        toAttack = nullptr;
    }

    virtual void moveTo(float dt, std::vector<BaseUnit*>& units);
    int getHp() {return this->hp;}
    void updateHpBar();

    void setActive(bool _active)
    {
        this->b_active = _active;

        if (this->b_active)
        {
            this->unit.setTexture(this->activeTexture);
        }

        else 
        {
            this->unit.setTexture(this->normalTexture);
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
    Team getTeam() const
    {
        return team;
    }

    bool GlobalBoundContainCheck(sf::Vector2i cord)
    {
        return unit.getGlobalBounds().contains(sf::Vector2f(cord));
    }

    void setAttacked(bool attacked)
    {
        this->attacked = attacked;
    }
    
    void move(float X, float Y)
    {
        unit.move(X / 64, Y / 64);
    }

    void doDamage(int damage, BaseUnit* unit, bool addNew = false)
    {
        this->hp -= damage;
        this->attacked = true;

        if (unit != nullptr && (!this->attack || addNew))
        {
            this->attack = true;
            this->toAttack = unit;

            this->clearTasks();
            
            TaskMove task;
            sf::Vector2f wayEnd;
            wayEnd.x = this->toAttack->xMap;
            wayEnd.y = this->toAttack->yMap;
            bool success = true;

            task = {this->predictPath(wayEnd, wayEnd.x, wayEnd.y, success), wayEnd};

            if (success)
            {
                this->tasks.push(task);
                this->setIsMoving(true);
            }

            else 
            {
                this->attack = false;
                this->toAttack = nullptr;
                this->clearTasks();
            }
        }
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

    sf::Sprite getShape() {return this->unit;}

    virtual void renderGame(sf::View view, bool renderPath);
    void renderMini(sf::View view);
    virtual void renderArrow();
};
