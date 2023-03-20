#include "BaseUnit.h"
#include <math.h>

#define PI 3.14159265

class Archer : public BaseUnit
{
    private:
        sf::RectangleShape range;
        std::vector<sf::CircleShape> pointsRange;

        sf::RectangleShape arrow;
        int distance;
        int maxDistance;
        bool shoot;
        int arrowSpeed;

        std::vector<sf::RectangleShape> predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success) override;
        void shootLogic();
        void updateRange();
        void updateArrow(float dt);

    public:
        Archer(sf::RenderWindow *_window, Tilemap* tilemap, int xMap, int yMap, std::vector<std::vector<int>>& mapUnits, Team _team) 
        : BaseUnit(_window, tilemap, xMap, yMap, mapUnits, _team)
        {
            this->range.setSize(sf::Vector2f(2000, 2000));
            this->range.setOrigin(this->range.getSize().x / 2, this->range.getSize().y / 2);
            this->range.setFillColor(sf::Color::Transparent);
            this->range.setOutlineThickness(2.0f);
            this->range.setOutlineColor(sf::Color::Green);

            this->arrow.setSize(sf::Vector2f(20, 5));
            this->arrow.setOrigin(this->arrow.getSize().x, this->arrow.getSize().y / 2);
            this->arrow.setFillColor(sf::Color::White);
            this->arrow.setOutlineThickness(2.0f);
            this->arrow.setOutlineColor(sf::Color::Green);
            this->arrow.setPosition(-1, -1);

            this->arrowSpeed = 500;
            this->damage = 40;
            this->maxHp = 100;
            this->hp = this->maxHp;
            this->speed = 2;
            this->speedAttack = 20;
            this->currentSpeedAttack = this->speedAttack;
            this->shoot = false;
            this->distance = 0;
            this->maxDistance = this->range.getSize().x / 2; // pixels
            this->toAttack = nullptr;

            for (int i = 0; i < 4; i++)
            {
                this->pointsRange.push_back(sf::CircleShape(10.0f));
            }

            this->updateRange();
        }

        void moveTo(float dt, std::vector<BaseUnit*>& units) override;
        void renderGame(sf::View view, bool renderPath) override;
        void renderArrow() override;
};