
#include "BaseUnit.h"

void BaseUnit::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, std::vector<std::vector<int>>& mapUnits, std::vector<std::vector<int>>& map)
{
    if (mousePressedLeft)
    {
        if (mousePressedLeft)
        {
            sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
            sf::Vector2f worldPos = this->window->mapPixelToCoords(mousepos);
            if (unit.getGlobalBounds().contains(worldPos))
            {
                this->b_active = true;
                this->setOutlineColor(255, 0 ,0);
            }
            else if (!find(pressedKeys, sf::Keyboard::LShift) || !find(pressedKeys, sf::Keyboard::RShift))
            {
                this->b_active = false;
                this->setOutlineColor(0, 0, 0);
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

            if (this->tasks.empty())
            {
                task = {this->predictPath(map, worldPos, worldPos.x, worldPos.y, success), worldPos};
            }

            else 
            {
                task = {this->predictPath(map, worldPos, this->tasks.back().wayEnd.x, this->tasks.back().wayEnd.y, success), worldPos};
            }

            if (success)
            {
                this->tasks.push(task);
            }

            this->setIsMoving(true);
        }
    }
}

void BaseUnit::clearTasks()
{
    while (!this->tasks.empty())
    {
        this->tasks.pop();
    }
}

std::vector<sf::RectangleShape> BaseUnit::predictPath(std::vector<std::vector<int>>& map, sf::Vector2f wayEnd, float& startX, float& startY, bool& success)
{
    int xPath;
    int yPath;

    int oldX;
    int oldY;
    
    if (this->tasks.empty())
    {
        xPath = this->xMap;
        yPath = this->yMap;

        oldX = this->xMap;
        oldY = this->yMap;
    }

    else 
    {
        xPath = startX;
        yPath = startY;

        oldX = startX;
        oldY = startY;
    }

    int previousDirectX = 0;
    int previousDirectY = 0;

    std::vector<sf::RectangleShape> path;

    while (true)
    {
        int directX = 0;
        int directY = 0;

        if (wayEnd.x > xPath)
        {
            directX = 1;
            xPath++;
        }

        if (wayEnd.x < xPath)
        {
            directX = -1;
            xPath--;
        }

        if (wayEnd.y > yPath)
        {
            directY = 1;
            yPath++;
        }

        if (wayEnd.y < yPath)
        {
            directY = -1;
            yPath--;
        }

        sf::RectangleShape shape(sf::Vector2f(5, 32));

        if (map[yPath][xPath] != 2)
        {
            success = false;
            return path;
        }

        if ((directX != previousDirectX || directY != previousDirectY) && path.size() != 0)
        {
            path[path.size() - 1].setSize(sf::Vector2f(32, 32));
            path[path.size() - 1].setOrigin(path[path.size() - 1].getSize().x / 2, path[path.size() - 1].getSize().y / 2);
            path[path.size() - 1].setRotation(0);
        }

        shape.setOrigin(shape.getSize().x / 2, shape.getSize().y / 2);
        shape.setPosition(sf::Vector2f(xPath * 64 + 32, yPath * 64 + 32));

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

        path.push_back(shape);

        if ((xPath == wayEnd.x && yPath == wayEnd.y))
        {
            path[path.size() - 1].setSize(sf::Vector2f(32, 32));
            path[path.size() - 1].setOrigin(path[path.size() - 1].getSize().x / 2, path[path.size() - 1].getSize().y / 2);
            path[path.size() - 1].setRotation(0);
            
            break;
        }

        previousDirectX = directX;
        previousDirectY = directY;

        oldX = xPath;
        oldY = yPath;
    }

    return path;
}

void BaseUnit::moveTo(std::vector<std::vector<int>>& mapUnits)
{
    if (!this->b_moving || this->tasks.empty() || this->tasks.front().path.size() == 0)
    {
        return;
    }

    if (this->currentSpeed != 0)
    {
        this->currentSpeed--;
        return;
    }

    int oldX = this->xMap;
    int oldY = this->yMap;

    std::vector<sf::RectangleShape>& path = this->tasks.front().path;
    sf::Vector2f wayEnd = this->tasks.front().wayEnd;

    this->xMap = path[0].getPosition().x / 64;
    this->yMap = path[0].getPosition().y / 64;

    if (mapUnits[this->yMap][this->xMap] == 1)
    {
        this->xMap = oldX;
        this->yMap = oldY;

        this->clearTasks();
        this->b_moving = false;

        return;
    }

    mapUnits[oldY][oldX] = 0;
    mapUnits[this->yMap][this->xMap] = 1;

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

    if (this->currentSpeed == 0)
    {
        this->currentSpeed = this->speed;
    }
}

void BaseUnit::renderGame(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);
    
    if (this->b_moving && !this->tasks.empty())
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
    this->window->setView(old);
}

void BaseUnit::renderMini(sf::View view)
{
    sf::View old = this->window->getView();
    this->window->setView(view);

    int xPos = this->unit.getPosition().x;
    int yPos = this->unit.getPosition().y;

    this->unit.setScale(0.15, 0.15);
    this->unit.setPosition(xPos * 0.15, yPos * 0.15);
    
    this->window->draw(this->unit);

    this->unit.setPosition(xPos, yPos);
    this->unit.setScale(1, 1);

    this->window->setView(old);
}
