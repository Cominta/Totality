#include "BaseUnit.h"

class Archer : public BaseUnit
{
    private:
        int range;

        std::vector<sf::RectangleShape> predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success) override;

    public:
        Archer(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits) 
        : BaseUnit(_window, tilemap, xMap, yMap, mapUnits)
        {
            this->damage = 10;
            this->maxHp = 100;
            this->hp = this->maxHp;
            this->range = 15;
            this->speed = 4;
            this->speedAttack = 0;
        }
};