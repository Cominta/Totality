#include "archerUnit.h"

std::vector<sf::RectangleShape> Archer::predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success)
{
    if (this->attack)
    {
        // left
        if (this->tilemap->map[wayEnd.y][wayEnd.x - this->range] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[wayEnd.y][wayEnd.x - this->range] <= this->tilemap->tileKeys["ground"].second)
        {
            wayEnd.x -= this->range;
        }

        // right
        else if (this->tilemap->map[wayEnd.y][wayEnd.x + this->range] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[wayEnd.y][wayEnd.x + this->range] <= this->tilemap->tileKeys["ground"].second)
        {
            wayEnd.x += this->range;
        }

        // top
        if (this->tilemap->map[wayEnd.y - this->range][wayEnd.x] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[wayEnd.y - this->range][wayEnd.x] <= this->tilemap->tileKeys["ground"].second)
        {
            wayEnd.y -= this->range;
        }

        // bottom
        else if (this->tilemap->map[wayEnd.y  + this->range][wayEnd.x] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[wayEnd.y + this->range][wayEnd.x] <= this->tilemap->tileKeys["ground"].second)
        {
            wayEnd.y += this->range;
        }
    }

    else 
    {
        bool suc = false;
        BaseUnit::predictPath(wayEnd, startX, startY, suc);
    }
}