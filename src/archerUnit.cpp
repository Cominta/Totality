#include "archerUnit.h"

std::vector<sf::RectangleShape> Archer::predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success)
{
    if (this->attack && this->toAttack != nullptr && this->range.getLocalBounds().intersects(this->toAttack->getShape().getGlobalBounds()))
    {
        return std::vector<sf::RectangleShape> ();
    }

    return BaseUnit::predictPath(wayEnd, startX, startY, success);
}

void Archer::shootLogic()
{    
    if (this->shoot && (this->arrow.getPosition().y / 64 < 0 || this->arrow.getPosition().x / 64 < 0 || 
        this->arrow.getPosition().y / 64 > this->tilemap->getHeight() || this->arrow.getPosition().x / 64 > this->tilemap->getWidth() ||
        this->tilemap->map[this->arrow.getPosition().y / 64][this->arrow.getPosition().x / 64] >= this->tilemap->tileKeys["mountain"].first ||
        this->toAttack == nullptr || 
        !this->range.getGlobalBounds().intersects(this->toAttack->getShape().getGlobalBounds())))
    {
        this->shoot = false;
        return;
    }

    float dx = this->unit.getPosition().x - this->toAttack->getShape().getPosition().x;
    float dy = this->unit.getPosition().y - this->toAttack->getShape().getPosition().y;
    float angle = std::atan2(dy, dx) * 180 / M_PI;
    
    int op = rand() % 100;

    if (op >= 50)
    {
        angle += rand() % (10 + 1 - 1) - 1;
    }

    else 
    {
        angle -= rand() % (10 + 1 - 1) - 1;
    }
    
    this->arrow.setRotation(angle);
    this->arrow.setPosition(this->unit.getPosition());
    this->shoot = true;
}

void Archer::updateRange()
{
    this->range.setPosition(this->unit.getPosition());

    this->pointsRange[0].setPosition(this->range.getPosition().x + this->range.getSize().x / 2, this->range.getPosition().y); // right
    this->pointsRange[1].setPosition(this->range.getPosition().x - this->range.getSize().x / 2, this->range.getPosition().y); // left
    this->pointsRange[2].setPosition(this->range.getPosition().x, this->range.getPosition().y - this->range.getSize().y / 2); // top
    this->pointsRange[3].setPosition(this->range.getPosition().x, this->range.getPosition().y + this->range.getSize().y / 2); // bottom
}

void Archer::updateArrow(float dt)
{
    if (!this->shoot || this->currentSpeedAttack > 0)
    {
        return;
    }

    float newX = -cos((PI / 180) * this->arrow.getRotation());
    float newY = -sin((PI / 180) * this->arrow.getRotation());

    this->arrow.move(newX * dt * this->arrowSpeed, newY * dt * this->arrowSpeed);

    if (this->arrow.getPosition().y / 64 > this->tilemap->getHeight() || this->arrow.getPosition().x / 64 > this->tilemap->getWidth())
    {
        this->arrow.setPosition(-1, -1);
        this->shoot = false;
        return;
    }

    if ((this->arrow.getPosition().x > this->unit.getPosition().x && this->arrow.getPosition().x - this->unit.getPosition().x > this->maxDistance) || 
        (this->arrow.getPosition().x < this->unit.getPosition().x && this->unit.getPosition().x - this->arrow.getPosition().x > this->maxDistance) ||
        (this->arrow.getPosition().y < this->unit.getPosition().y && this->unit.getPosition().y - this->arrow.getPosition().y > this->maxDistance) || 
        (this->arrow.getPosition().y > this->unit.getPosition().y && this->arrow.getPosition().y - this->unit.getPosition().y > this->maxDistance) ||
        this->tilemap->map[this->arrow.getPosition().y / 64][this->arrow.getPosition().x / 64] >= this->tilemap->tileKeys["mountain"].first)
    {
        this->shootLogic();
    }
}

void Archer::moveTo(float dt, std::vector<BaseUnit*>& units)
{
    this->updateRange();
    this->updateArrow(dt);

    if (!this->attack)
    {
        this->arrow.setPosition(-1, -1);
        this->shoot = false;
        this->toAttack = nullptr;
    }

    if (this->toAttack != nullptr && this->toAttack->getHp() <= 0)
    {
        this->attack = false;
        this->shoot = false;
        this->toAttack = nullptr;
    }

    if (this->toAttack != nullptr && !this->range.getGlobalBounds().intersects(this->toAttack->getShape().getGlobalBounds()))
    {
        this->shoot = false;
    }

    if (this->attack && this->toAttack != nullptr && this->team != this->toAttack->getTeam())
    {
        if (this->tilemap->mapUnits[this->arrow.getPosition().y / 64][this->arrow.getPosition().x / 64] == 1)
        {
            for (auto unit : units)
            {
                if (unit == nullptr)
                {
                    continue;
                }

                if (unit != nullptr && unit->getHp() <= 0)
                {
                    continue;
                }

                if (this->arrow.getGlobalBounds().intersects(unit->getShape().getGlobalBounds()) && this->team != unit->getTeam())
                {
                    this->arrow.setPosition(-1, -1);
                    this->shoot = false;

                    unit->doDamage(this->damage, this);
                }
            }
        }

        if (this->attack && this->toAttack != nullptr && this->range.getGlobalBounds().intersects(this->toAttack->getShape().getGlobalBounds()))
        {
            this->clearTasks();

            if (this->currentSpeedAttack > 0)
            {
                this->currentSpeedAttack--;
            }

            if (!this->shoot && this->currentSpeedAttack <= 0)
            {
                this->currentSpeedAttack = this->speedAttack + (1.0f / dt);
                this->arrow.setPosition(this->unit.getPosition());
                this->shoot = true;
                this->shootLogic();
            }

            if (this->arrow.getGlobalBounds().intersects(this->toAttack->getShape().getGlobalBounds()))
            {
                this->arrow.setPosition(-1, -1);
                this->shoot = false;

                this->toAttack->doDamage(this->damage, this);
            }

            return;
        }
    }

    if (this->tasks.empty() || this->tasks.front().path.size() == 0)
    {
        return;
    }

    if (this->currentSpeed > 0)
    {
        this->currentSpeed -= 2;
        return;
    }

    if (this->newPredict())
    {
        this->clearTasks();
        this->b_moving = false;
        this->attack = false;
        
        return;
    }

    int oldX = this->xMap;
    int oldY = this->yMap;

    std::vector<sf::RectangleShape>& path = this->tasks.front().path;
    sf::Vector2f wayEnd = this->tasks.front().wayEnd;

    int newX = path[0].getPosition().x / 64;
    int newY = path[0].getPosition().y / 64;

    this->updateHpBar();

    if (this->tilemap->map[newY][newX] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[newY][newX] <= this->tilemap->tileKeys["sand"].second)
    {
        this->slowed = true;
    }

    if (this->tilemap->mapUnits[newY][newX] == 1 && (!this->attack || (this->attack && this->toAttack != nullptr && this->toAttack->xMap != newX || this->toAttack->yMap != newY)))
    {
        // this->xMap = oldX;
        // this->yMap = oldY;

        this->clearTasks();
        this->b_moving = false;

        return;
    }

    // else if (this->attack && this->toAttack != nullptr && this->toAttack->xMap == newX && this->toAttack->yMap == newY)
    // {
    //     if (this->currentSpeedAttack != 0)
    //     {
    //         this->currentSpeedAttack--;
    //         return;
    //     }

    //     this->b_moving = false;
    //     // this->xMap = oldX;
    //     // this->yMap = oldY;

    //     // this->toAttack->doDamage(this->damage);
    //     this->currentSpeedAttack = this->speedAttack + (1.0f / dt) / 10;

    //     // if (this->toAttack->hp <= 0)
    //     // {
    //     //     this->attack = false;
    //     //     this->toAttack = nullptr;
    //     // }

    //     return;
    // }

    sounds::play("walk", sounds::getVolume("walk"), false, true, this->unit.getPosition().x, this->unit.getPosition().y);

    this->xMap = newX;
    this->yMap = newY;

    this->tilemap->mapUnits[oldY][oldX] = 0;
    this->tilemap->mapUnits[this->yMap][this->xMap] = 1;

    this->unit.setPosition(this->xMap * 64 + 32, this->yMap * 64 + 32);
    path.erase(path.begin());

    if (wayEnd.x == this->xMap && wayEnd.y == this->yMap)
    {
        this->tasks.pop();

        if (this->tasks.empty())
        {
            this->b_moving = false;
        }
    }

    if (this->slowed)
    {
        this->currentSpeed = this->speed + (1.0f / dt * 1.5) + (1.0f / dt);
        this->slowed = false;
    }

    else 
    {
        this->currentSpeed = this->speed + (1.0f / dt); // 1.0f / dt = fps
    }
}

void Archer::renderGame(sf::View view, bool renderPath)
{
    BaseUnit::renderGame(view, renderPath);

    if (this->b_active)
    {
        // this->window->draw(this->range); // debug
        for (auto point : pointsRange)
        {
            this->window->draw(point);
        }
    }
}

void Archer::renderArrow()
{
    if (this->shoot)
    {
        this->window->draw(this->arrow);
    }
}