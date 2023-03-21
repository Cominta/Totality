#pragma once

#include "BaseUnit.h"

class Warrior : public BaseUnit
{
    private:

    public:
        Warrior(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits, Team _team, sf::Texture texture) 
        : BaseUnit(_window, tilemap, xMap, yMap, mapUnits, _team, texture)
        {
            this->damage = 20;
            this->maxHp = 200;
            this->hp = this->maxHp;
        }
};