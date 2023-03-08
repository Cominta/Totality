#pragma once

#include "BaseUnit.h"

class Warrior : public BaseUnit
{
private:

public:
    Warrior(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits) 
    : BaseUnit(_window, tilemap, xMap, yMap, mapUnits)
    {}
};