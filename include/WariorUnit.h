#pragma once

#include "BaseUnit.h"

class Warrior : public BaseUnit
{
private:

public:
    Warrior(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, int _observationR , int _atackDMG, int _defence, std::vector<std::vector<int>>& mapUnits) 
    : BaseUnit(_window, tilemap, xMap, yMap, _observationR, _atackDMG, _defence, &mapUnits)
    {}
};