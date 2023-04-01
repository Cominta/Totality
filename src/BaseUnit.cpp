
#include "BaseUnit.h"

void BaseUnit::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& realisedKeys, std::vector<int>& pressedKeys, std::vector<BaseUnit*>& units)
{
    srand(time(0));

    this->updateHpBar();

    if (mousePressedLeft)
    {
        if (mousePressedLeft)
        {
            sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
            sf::Vector2f worldPos = this->window->mapPixelToCoords(mousepos);
            if (unit.getGlobalBounds().contains(worldPos))
            {
                this->setActive(true);
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
            {
                this->setActive(false);
            }
        }
    }
    else if (mousePressedRight)
    {
        if (isActiv())
        {
            sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
            sf::Vector2f worldPos = this->window->mapPixelToCoords(mousepos);

            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                this->clearTasks();
            }

            worldPos.x /= 64;
            worldPos.y /= 64;
            worldPos.x = (float)(int)worldPos.x;
            worldPos.y = (float)(int)worldPos.y;

            TaskMove task;
            bool success = true;

            if ((worldPos.x > this->tilemap->getWidth() - 1 || worldPos.y > this->tilemap->getHeight() - 1) ||
                (this->tilemap->map[worldPos.y][worldPos.x] < this->tilemap->tileKeys["sand"].first || this->tilemap->map[worldPos.y][worldPos.x] > this->tilemap->tileKeys["ground"].second && 
                this->tilemap->map[worldPos.y][worldPos.x] < this->tilemap->tileKeys["crossGround"].first))
            {
                return;
            }

            if (this->tilemap->mapUnits[worldPos.y][worldPos.x] == 1 && (this->xMap != worldPos.x || this->yMap != worldPos.y))
            {
                this->attack = true;
                this->clearTasks();
                
                for (auto unit : units)
                {
                    if (unit->xMap == worldPos.x && unit->yMap == worldPos.y && unit->getTeam() == this->team && unit != this)
                    {
                        this->attack = false;
                        break;
                    }

                    if (unit->xMap == worldPos.x && unit->yMap == worldPos.y && unit->getTeam() != this->team)
                    {
                        this->toAttack = unit;
                        worldPos.x = this->toAttack->xMap;
                        worldPos.y = this->toAttack->yMap;

                        // if (abs(this->xMap - worldPos.x) > 1)
                        // {
                        //     if (this->xMap < worldPos.x)
                        //     {
                        //         worldPos.x--;
                        //     }

                        //     if (this->xMap > worldPos.x)
                        //     {
                        //         worldPos.x++;
                        //     }
                        // }

                        // if (abs(this->yMap - worldPos.y) > 1)
                        // {
                        //     if (this->yMap < worldPos.y)
                        //     {
                        //         worldPos.y--;
                        //     }

                        //     if (this->yMap > worldPos.y)
                        //     {
                        //         worldPos.y++;
                        //     }
                        // }
                    }
                }
                
            }

            else 
            {
                this->attack = false;
                // this->toAttack = nullptr;
            }

            if (this->tasks.empty())
            {
                task = {this->predictPath(worldPos, worldPos.x, worldPos.y, success), worldPos};
            }

            else 
            {
                task = {this->predictPath(worldPos, this->tasks.back().wayEnd.x, this->tasks.back().wayEnd.y, success), worldPos};
            }

            if (success)
            {
                this->tasks.push(task);
                this->setIsMoving(true);
            }
        }
    }
}

sf::Texture BaseUnit::editSprite(sf::Color color, sf::Texture texture)
{
    sf::Image image = texture.copyToImage();

    for (int y = 0; y < image.getSize().y; y++)
    {
        for (int x = 0; x < image.getSize().x; x++)
        {
            sf::Color pixel = image.getPixel(x, y);

            if (pixel.r == 77 && pixel.g == 255 && pixel.b == 0)
            {
                image.setPixel(x, y, color);
            }
        }
    }

    sf::Texture result;
    result.loadFromImage(image);
    
    return result;
}

void BaseUnit::updateHpBar()
{
    this->hpBar.setSize(sf::Vector2f(32 * ((float)this->hp / this->maxHp), 6));
    this->hpBar.setPosition(this->unit.getPosition().x, this->unit.getPosition().y - 30);

    this->hpBarBack.setPosition(this->unit.getPosition().x, this->unit.getPosition().y - 30);
}

void BaseUnit::initHpBar()
{
    this->hpBar.setSize(sf::Vector2f(32, 6));
    this->hpBar.setOrigin(16, 20);
    this->hpBar.setFillColor(sf::Color::Green);

    this->hpBarBack.setSize(sf::Vector2f(32, 6));
    this->hpBarBack.setOrigin(16, 20);
    this->hpBarBack.setFillColor(sf::Color::Black);
}

void BaseUnit::clearTasks()
{
    while (!this->tasks.empty())
    {
        this->tasks.pop();
    }
}

std::vector<sf::RectangleShape> BaseUnit::predictPath(sf::Vector2f wayEnd, float& startX, float& startY, bool& success)
{
    int xPath;
    int yPath;
    
    if (this->tasks.empty())
    {
        xPath = this->xMap;
        yPath = this->yMap;
    }

    else 
    {
        xPath = startX;
        yPath = startY;
    }

    std::vector<sf::RectangleShape> pathResult;

    AStar::CoordinateList pathGenerate = this->generator.findPath({xPath, yPath}, {(int)wayEnd.x, (int)wayEnd.y});
    AStar::CoordinateList pathReverse;

    for (int i = pathGenerate.size() - 2; i >= 0; i--)
    {
        pathReverse.push_back(pathGenerate[i]);
    }

    pathGenerate = pathReverse;

    if (pathGenerate[pathGenerate.size() - 1].x >= 99 || pathGenerate[pathGenerate.size() - 1].y >= 99)
    {
        success = false;
        return std::vector<sf::RectangleShape> ();
    }

    int oldX = this->xMap;
    int oldY = this->yMap;

    int previousDirectX = 0;
    int previousDirectY = 0;

    for (auto coord : pathGenerate)
    {
        int directX = 0;
        int directY = 0;

        sf::RectangleShape shape(sf::Vector2f(5, 64));
        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
        shape.setPosition(coord.x * 64 + 32, coord.y * 64 + 32);
        
        if (coord.x > oldX) // right
        {
            directX = 1;
        }

        else if (coord.x < oldX) // left
        {
            directX = -1;
        }

        if (coord.y < oldY) // top
        {
            directY = -1;
        }

        else if (coord.y > oldY) // bottom
        {
            directY = 1;
        }

        if ((directX != previousDirectX || directY != previousDirectY) && pathResult.size() != 0)
        {
            pathResult[pathResult.size() - 1].setSize(sf::Vector2f(32, 32));
            pathResult[pathResult.size() - 1].setOrigin(pathResult[pathResult.size() - 1].getSize().x / 2, pathResult[pathResult.size() - 1].getSize().y / 2);
            pathResult[pathResult.size() - 1].setRotation(0);
        }

        if (directY == -1 && directX == -1) // top - left
        {
            shape.setRotation(-45);
        }

        else if (directY == -1 && directX == 1) // top - right
        {
            shape.setRotation(45);
        }

        else if (directY == 1 && directX == -1) // bottom - left
        {
            shape.setRotation(-135);
        }

        else if (directY == 1 && directX == 1) // bottom - right
        {
            shape.setRotation(135);
        }

        else if (directY == 0) // left - right
        {
            shape.setRotation(90 * directX);
        }

        else if (directX == 0) // top - bottom
        {
            shape.setRotation(180 * directY);
        }

        oldX = coord.x;
        oldY = coord.y;

        previousDirectX = directX;
        previousDirectY = directY;

        pathResult.push_back(shape);
    }

    pathResult[pathResult.size() - 1].setSize(sf::Vector2f(32, 32));
    pathResult[pathResult.size() - 1].setOrigin(pathResult[pathResult.size() - 1].getSize().x / 2, pathResult[pathResult.size() - 1].getSize().y / 2);
    pathResult[pathResult.size() - 1].setRotation(0);

    return pathResult;
}

bool BaseUnit::newPredict()
{
    if (this->attack && this->toAttack != nullptr && this->toAttack->hp > 0)
    {
        this->clearTasks();
        float startX = (float)this->xMap;
        float startY = (float)this->yMap;
        bool success = true;
        TaskMove task {this->predictPath(sf::Vector2f(this->toAttack->xMap, this->toAttack->yMap), startX, startY, success), sf::Vector2f(this->toAttack->xMap, this->toAttack->yMap)};
        this->tasks.push(task);

        return !success;
    }

    return false;
}

void BaseUnit::flipTexture(int newX, int newY)
{
    if (newX > this->xMap && this->unit.getScale().x != 1)
    {
        this->unit.setScale(1, 1);
    }

    else if (newX < this->xMap && this->unit.getScale().x == 1) 
    {
        this->unit.setScale(-1, 1);
    }
}

void BaseUnit::moveTo(float dt, std::vector<BaseUnit*>& units)
{
    if (!this->attack)
    {
        this->toAttack = nullptr;
    }

    if (this->toAttack == nullptr)
    {
        this->attack = false;
    }

    if (this->attack && this->toAttack != nullptr && this->toAttack->hp <= 0)
    {
        this->attack = false;
        this->toAttack = nullptr;

        this->clearTasks();
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

    this->flipTexture(newX, newY);
    
    if (this->tilemap->mapUnits[newY][newX] == 1)
    {
        for (auto unit : units)
        {
            if (unit != nullptr && unit->xMap == newX && unit->yMap == newY && unit->getTeam() != this->team)
            {
                this->doDamage(0, unit, true);
                this->attacked = false;
            }
        }
    }

    if (this->tilemap->map[newY][newX] >= this->tilemap->tileKeys["sand"].first && this->tilemap->map[newY][newX] <= this->tilemap->tileKeys["sand"].second ||
    this->tilemap->map[newY][newX] >= this->tilemap->tileKeys["crossSand"].first && this->tilemap->map[newY][newX] <= this->tilemap->tileKeys["crossSand"].second)
    {
        this->slowed = true;
    }

    if (this->tilemap->mapUnits[newY][newX] == 1 && ((!this->attack || 
        (this->attack && this->toAttack != nullptr && this->toAttack->xMap != newX || this->toAttack->yMap != newY))))
    {
        // this->xMap = oldX;
        // this->yMap = oldY;

        // this->clearTasks();
        // this->b_moving = false;

        return;
    }

    else if (this->attack && this->toAttack != nullptr && this->toAttack->xMap == newX && this->toAttack->yMap == newY)
    {
        if (this->currentSpeedAttack > 0)
        {
            this->currentSpeedAttack--;
            return;
        }

        this->b_moving = false;
        // this->xMap = oldX;
        // this->yMap = oldY;
        if (this->toAttack != nullptr && this->team != this->toAttack->getTeam())
        {
            int hitSound = randomNumbers::getRandomNum(1, 2);
            std::string str = "hit_sword_" + std::string(std::to_string(hitSound));
            sounds::play(str, sounds::getVolume(str), false, true, this->unit.getPosition().x, this->unit.getPosition().y);
            
            this->toAttack->doDamage(this->damage, this);
            this->currentSpeedAttack = this->speedAttack + (1.0f / dt) / 10;
        }

        return;
    }

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

void BaseUnit::renderGame(sf::View view, bool renderPath)
{
    sf::View old = this->window->getView();
    this->window->setView(view);
    
    if (this->b_moving && !this->tasks.empty() && renderPath)
    {
        std::queue<TaskMove> temp = this->tasks;

        while (!temp.empty())
        {
            for (int i = 0; i < temp.front().path.size(); i++)
            {
                this->window->draw(temp.front().path[i]);
            }

            temp.pop();
        }
    }

    this->window->draw(this->unit);

    this->window->draw(this->hpBarBack);
    this->window->draw(this->hpBar);

    this->window->setView(old);
}

void BaseUnit::renderMini(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);

    int xPos = this->unit.getPosition().x;
    int yPos = this->unit.getPosition().y;

    sf::CircleShape shape;
    shape.setRadius(32);
    shape.setScale(0.30, 0.30);
    shape.setPosition(xPos * 0.15, yPos * 0.15);
    shape.setOrigin(32, 32);
    
    this->window->draw(shape);
    this->window->setView(old);
}

void BaseUnit::renderArrow()
{
    
}