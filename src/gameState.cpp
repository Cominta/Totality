#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures, unsigned int seed)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    srand(time(0));
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 3, 8, seed);
    this->camera = new Camera(this->window, 16000, 16000);
    this->gameView = this->window->getView();
    this->minimap.setViewport(sf::FloatRect(0.86f, 0, 0.15f, 0.25f));

    this->buttons["AddUnit"] = new Button(this->window, 1840, 1000, 1, &textures["UnitButton_Idle"]);
    this->buttons["AddBaseUnit"] = new Button(this->window, 1840, 920, 1, &textures["BaseUnitAddButton_Idle"]);
    this->buttons["AddUnit"]->setActiv(false);
    this->buttons["AddBaseUnit"]->setActiv(false);
    this->multiply = false;
    // sf::Color color();
    // color.a = 100;
    this->multiplyShape.setFillColor(sf::Color::Transparent);
    this->multiplyShape.setOutlineColor(sf::Color(17, 255, 0));
    this->multiplyShape.setOutlineThickness(5.0f);
}

GameState::~GameState()
{
    for (auto button : this->buttons)
    {
        delete button.second;
    }

    for (auto unit : this->units)
    {
        if (unit != nullptr)
        {
            delete unit;
        }
    }

    this->units.clear();

    this->window->setView(this->window->getDefaultView());
}

void GameState::generateBlood(std::pair<sf::Vector2f, int>& pos)
{
    int x = pos.first.x;
    int y = pos.first.y;

    int op = rand() % 100;

    if (op >= 50)
    {
        x += rand() % (40 + 1 - 5) - 5;
    }

    else 
    {
        x -= rand() % (40 + 1 - 5) - 5;
    }

    op = rand() % 100;

    if (op >= 50)
    {
        y += rand() % (40 + 1 - 5) - 5;
    }

    else 
    {
        y -= rand() % (40 + 1 - 5) - 5;
    }

    pos.first.x = x;
    pos.first.y = y;
    pos.second = rand() % (360 + 1 - 0) - 0;
}

void GameState::updateUnits(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    this->updateButtons(mousePressedLeft);
    if (buttons.at("AddBaseUnit")->isActiv())
    {
        if (mousePressedLeft)
        {
            if (this->tilemap->map[mousePosition.y / 64][mousePosition.x / 64] >= this->tilemap->tileKeys["ground"].first 
            && this->tilemap->map[mousePosition.y / 64][mousePosition.x / 64] <= this->tilemap->tileKeys["ground"].second)
            {
                if (this->tilemap->mapUnits[mousePosition.y / 64][mousePosition.x / 64] != 1)
                {
                    if (!buttons.at("AddBaseUnit")->isHover(sf::Vector2f(this->mousePosition.x, this->mousePosition.y)))
                    {
                        this->units.push_back(new BaseUnit(this->window, this->tilemap, mousePosition.x / 64, mousePosition.y / 64, 
                        this->tilemap->mapUnits));
                    }
                }
            }
        }
    }


    for (int i = 0; i < this->units.size(); i++)
    {
        if (this->units[i] == nullptr)
        {
            continue;
        }

        if (this->units[i]->getHp() <= 0)
        {
            this->tilemap->mapUnits[this->units[i]->getY()][this->units[i]->getX()] = 0;

            if (this->units[i] != nullptr)
            {
                delete this->units[i];
            }

            this->units.erase(this->units.begin() + i);
            break;
        }

        if (this->units[i]->getAttacked())
        {
            std::pair<sf::Vector2f, int> blood;

            sf::Vector2f pos(this->units[i]->getX() * 64, this->units[i]->getY() * 64);
            blood.first = pos;
            
            this->generateBlood(blood);
            this->bloods.push_back(blood);
        }

        this->units[i]->update(mousePressedLeft, mousePressedRight, realisedKeys, pressedKeys, this->units);
        this->units[i]->moveTo();
    }
}

void GameState::updateButtons(bool mousePressedLeft)
{
    //this->window->mapPixelToCoords(sf::Vector2i(10, 20));
    this->updateMouse();
    if (buttons.at("AddUnit")->isHover(this->mousePosition) && mousePressedLeft)
    {
        if (buttons.at("AddUnit")->isActiv())
        {
            for (auto it : this->buttons)
            {
                it.second->setActiv(false);
            }
        }
        else
        {
            buttons.at("AddUnit")->setActiv(true);
        }
    }

    int counter = 0;
    for(auto& it : this->buttons)
    {
        it.second->setPosition(this->window->mapPixelToCoords(sf::Vector2i(0, 1000 - 80 * counter)));
        it.second->setScale(this->window->getView().getSize().x / 100 / 18.0f, this->window->getView().getSize().y / 100 / 10);
        counter++;
    }

    if (buttons.at("AddUnit")->isActiv())
    {
        if (buttons.at("AddBaseUnit")->isHover(this->mousePosition) && mousePressedLeft)
        {
            if (buttons.at("AddBaseUnit")->isActiv())
            {
                buttons.at("AddBaseUnit")->setActiv(false);
            }
            else
            {
                buttons.at("AddBaseUnit")->setActiv(true);
            }
        }
    }
}

void GameState::multiplyUnits()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !this->multiply && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        this->multiply = true;
        this->multiplyShape.setPosition(this->mousePosition.x, this->mousePosition.y);
        this->mStartPos = this->mousePosition;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && this->multiply && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        if (this->mStartPos.x < this->mousePosition.x && this->mStartPos.y < this->mousePosition.y) // bottom - right
        {
            this->multiplyShape.setSize(sf::Vector2f(this->mousePosition.x - this->mStartPos.x, this->mousePosition.y - this->mStartPos.y));
            this->multiplyShape.setPosition(this->mStartPos.x, this->mStartPos.y);
        }

        else if (this->mStartPos.x > this->mousePosition.x && this->mStartPos.y < this->mousePosition.y) // bottom - left
        {
            this->multiplyShape.setSize(sf::Vector2f(this->mStartPos.x - this->mousePosition.x, this->mousePosition.y - this->mStartPos.y));
            this->multiplyShape.setPosition(this->mousePosition.x, this->mStartPos.y);
        }

        else if (this->mStartPos.x < this->mousePosition.x && this->mStartPos.y > this->mousePosition.y) // top - right
        {
            this->multiplyShape.setSize(sf::Vector2f(this->mousePosition.x - this->mStartPos.x, this->mStartPos.y - this->mousePosition.y));
            this->multiplyShape.setPosition(this->mStartPos.x, this->mousePosition.y);
        }

        else if (this->mStartPos.x > this->mousePosition.x && this->mStartPos.y > this->mousePosition.y) // top - left
        {
            this->multiplyShape.setSize(sf::Vector2f(this->mStartPos.x - this->mousePosition.x, this->mStartPos.y - this->mousePosition.y));
            this->multiplyShape.setPosition(this->mousePosition.x, this->mousePosition.y);
        }
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && this->multiply && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
    {
        for (auto unit : this->units)
        {
            if (this->multiplyShape.getGlobalBounds().intersects(unit->getShape().getGlobalBounds()))
            {
                unit->setActive(true);
            }
        }

        this->multiply = false;
        this->multiplyShape.setSize(sf::Vector2f(0, 0));
    }
}

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll)
{
    this->updateMouse();
    this->camera->update(this->mousePosition, mouseScroll, sizeMapX, sizeMapY);
    this->gameView = this->window->getView();

    this->multiplyUnits();
    this->updateUnits(mousePressedLeft, mousePressedRight, pressedKeys, realisedKeys);

    if (this->find(realisedKeys, sf::Keyboard::Key::Escape))
    {
        delete this->states.top();
        this->states.pop();
        return;
    }
}

void GameState::render()
{
    this->tilemap->renderGame(this->gameView);

    for (auto blood : this->bloods)
    {
        sf::Sprite sprite;

        sprite.setOrigin(this->textures["blood"].getSize().x / 2, this->textures["blood"].getSize().y / 2);
        sprite.setTexture(this->textures["blood"]);
        sprite.setPosition(blood.first.x, blood.first.y);
        sprite.setRotation(blood.second);

        this->window->draw(sprite);
    }
    
    for (auto& unit : this->units)
    {
        unit->renderGame(this->gameView);
    }

    if (this->multiply)
    {
        this->window->draw(this->multiplyShape);
    }
    
    this->tilemap->renderMini(this->minimap);

    for (auto& unit : this->units)
    {
        unit->renderMini(this->minimap);
    }
    
    if (buttons.at("AddUnit")->isActiv())
    {
        for (auto& button : this->buttons)
        {
            button.second->render();
        }
    }
    else
    {
        buttons.at("AddUnit")->render();
    }
}