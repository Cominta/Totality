#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures, unsigned int seed)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    srand(time(0));
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 3, 8, seed);
    this->camera = new Camera(this->window, 16000, 16000);
    this->gameView = this->window->getView();
    this->minimap.setViewport(sf::FloatRect(0.86f, 0, 0.15f, 0.25f));
    this->minimapCamera.setSize(this->gameView.getSize());
    this->minimapCamera.setFillColor(sf::Color::Transparent);
    this->minimapCamera.setOutlineThickness(50.0f);

    this->buttons["Prepare"] = new Button(this->window, 910, 0, 1, &textures["Prepare_End"]);
    this->buttons["Prepare"]->setOrigin(textures["Prepare_End"].getSize().x / 2, textures["Prepare_End"].getSize().y / 2);
    this->buttons["AddUnit"] = new Button(this->window, 1840, 1000, 1, &textures["UnitButton_Idle"]);
    this->buttons["Team"] = new Button(this->window, 1840, 920, 1, &textures["Team_Red"]);
    this->buttons["AddBaseUnit"] = new Button(this->window, 1840, 840, 1, &textures["BaseUnitAddButton_Idle"]);
    this->buttons["AddArcherUnit"] = new Button(this->window, 1840, 660, 1, &textures["ArcherUnitAddButton_Idle"]);
    this->buttons["AddUnit"]->setActiv(false);
    this->buttons["AddBaseUnit"]->setActiv(false);
    this->buttons["AddArcherUnit"]->setActiv(false);
    this->buttons["Prepare"]->setActiv(true);
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

    if (op >= 45)
    {
        x += abs(rand() % (100 + 1 - 5) - 5);
    }

    else 
    {
        x -= abs(rand() % (100 + 1 - 5) - 5);
    }

    op = rand() % 100;

    if (op >= 45)
    {
        y += abs(rand() % (100 + 1 - 5) - 5);
    }

    else 
    {
        y -= abs(rand() % (100 + 1 - 5) - 5);
    }

    pos.first.x = x;
    pos.first.y = y;
    pos.second = rand() % (360 + 1 - 0) - 0;
}

void GameState::updateUnits(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, float dt)
{
    int team = this->updateButtons(mousePressedLeft);

    for (auto& unit : this->units)
    {
        unit->updateHpBar();
    }

    if (buttons.at("Prepare")->isActiv() && this->tilemap->map[mousePosition.y / 64][mousePosition.x / 64] >= this->tilemap->tileKeys["ground"].first 
    && this->tilemap->map[mousePosition.y / 64][mousePosition.x / 64] <= this->tilemap->tileKeys["ground"].second)
    {
        if (this->tilemap->mapUnits[mousePosition.y / 64][mousePosition.x / 64] != 1 && mousePressedLeft)
        {
            if (!buttons.at("AddBaseUnit")->isHover(sf::Vector2f(this->mousePosition.x, this->mousePosition.y)) && buttons.at("AddBaseUnit")->isActiv())
            {
                bool add = true;

                for (auto button : this->buttons)
                {
                    if (button.first != "AddBaseUnit" && button.second->isHover(sf::Vector2f(this->mousePosition.x, this->mousePosition.y)))
                    {
                        add = false;
                        // buttons.at("AddBaseUnit")->setActiv(false);
                        break;
                    }
                }
                
                if (add && buttons.at("AddBaseUnit")->isActiv())
                {
                    this->units.push_back(new Warrior(this->window, this->tilemap, mousePosition.x / 64, mousePosition.y / 64, this->tilemap->mapUnits, Team(team)));
                }
            }

            else if (!buttons.at("AddArcherUnit")->isHover(sf::Vector2f(this->mousePosition.x, this->mousePosition.y)) && buttons.at("AddArcherUnit")->isActiv())
            {
                bool add = true;

                for (auto button : this->buttons)
                {
                    if (button.first != "AddArcherUnit" && button.second->isHover(sf::Vector2f(this->mousePosition.x, this->mousePosition.y)))
                    {
                        add = false;
                        break;
                    }
                }

                if (add && buttons.at("AddArcherUnit")->isActiv())
                {
                    this->units.push_back(new Archer(this->window, this->tilemap, mousePosition.x / 64, mousePosition.y / 64, this->tilemap->mapUnits, Team(team)));
                }
            }
        }
    }

    if (!buttons.at("Prepare")->isActiv())
    {
        for (int i = 0; i < this->units.size(); i++)
        {
            if (this->units[i] == nullptr)
            {
                continue;
            }

            if (this->units[i]->getHp() <= 0)
            {
                std::pair<sf::Vector2f, int> dead;

                sf::Vector2f pos(this->units[i]->getX() * 64, this->units[i]->getY() * 64);
                dead.first = pos;
                
                this->generateBlood(dead);
                this->deads.push_back(dead);

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
            this->units[i]->moveTo(dt, this->units);
        }
    }
}

int GameState::updateButtons(bool mousePressedLeft)
{
    static int team = 0;
    //this->window->mapPixelToCoords(sf::Vector2i(10, 20));
    this->updateMouse();
    if (buttons.at("Prepare")->isHover(this->mousePosition) && mousePressedLeft)
    {
        if (buttons.at("Prepare")->isActiv())
        {
            buttons.at("Prepare")->setActiv(false);
            buttons.at("Prepare")->setTexture(&textures["Prepare_Reset"]);
            for (auto it : this->buttons)
            {
                it.second->setActiv(false);
            }
        }
        else
        {
            for (auto unit : this->units)
            {
                delete unit;
            }

            this->units.clear();
            this->bloods.clear();
            this->deads.clear();

            buttons.at("Prepare")->setActiv(true);
            buttons.at("Prepare")->setTexture(&textures["Prepare_End"]);
        }
        buttons.at("Prepare")->updateSprite();
    }

    int counter = 1;

    for(auto& it : this->buttons)
    {
        if (it.first == "Prepare")
        {
            it.second->setPosition(this->window->mapPixelToCoords(sf::Vector2i(this->window->getSize().x / 2, 
                                    80)));
            it.second->setScale(this->window->getView().getSize().x / 100 / 18.0f, this->window->getView().getSize().y / 100 / 10);
        }

        else 
        {
            it.second->setPosition(this->window->mapPixelToCoords(sf::Vector2i(0, this->window->getSize().y - 80 * counter)));
            it.second->setScale(this->window->getView().getSize().x / 100 / 18.0f, this->window->getView().getSize().y / 100 / 10);
            counter++;
        }
    }

    if (buttons.at("Prepare")->isActiv())
    {
        if (buttons.at("AddUnit")->isHover(this->mousePosition) && mousePressedLeft)
        {
            if (buttons.at("AddUnit")->isActiv())
            {
                for (auto it : this->buttons)
                {
                    it.second->setActiv(false);
                }
                buttons.at("Prepare")->setActiv(true);
            }
            else
            {
                buttons.at("AddUnit")->setActiv(true);
            }
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
                    for (auto it : this->buttons)
                    {
                        it.second->setActiv(false);
                    }
                    buttons.at("AddUnit")->setActiv(true);
                    buttons.at("AddBaseUnit")->setActiv(true);
                }
            }
            else if(buttons.at("AddArcherUnit")->isHover(this->mousePosition) && mousePressedLeft)
            {
                if (buttons.at("AddArcherUnit")->isActiv())
                {
                    buttons.at("AddArcherUnit")->setActiv(false);
                }
                else
                {
                    for (auto it : this->buttons)
                    {
                        it.second->setActiv(false);
                    }
                    buttons.at("AddUnit")->setActiv(true);
                    buttons.at("AddArcherUnit")->setActiv(true);
                }
            }
            else if (buttons.at("Team")->isHover(this->mousePosition) && mousePressedLeft)
            {
                team++;
                team = team % 4;
                if (team == 0)
                {
                    buttons.at("Team")->setTexture(&textures["Team_Red"]);
                }
                else if (team == 1)
                {
                    buttons.at("Team")->setTexture(&textures["Team_Blue"]);
                }
                else if (team == 2)
                {
                    buttons.at("Team")->setTexture(&textures["Team_Green"]);
                }
                else if (team == 3)
                {
                    buttons.at("Team")->setTexture(&textures["Team_Yellow"]);
                }
                buttons.at("Team")->updateSprite();
                return team;
            }
        }
        buttons.at("Prepare")->setActiv(true);
    }
    return team;
}

void GameState::multiplyUnits()
{
    if (this->buttons["Prepare"]->isActiv())
    {
        return;
    }

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

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || this->multiply && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
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

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll, float dt)
{
    this->updateMouse();
    this->camera->update(this->mousePosition, mouseScroll, sizeMapX, sizeMapY, dt);
    this->gameView = this->window->getView();

    this->minimapCamera.setSize(this->gameView.getSize());
    this->minimapCamera.setScale(this->minimap.getViewport().width / this->gameView.getViewport().width, 
                             this->minimap.getViewport().width / this->gameView.getViewport().width);

    this->minimapCamera.setPosition(this->gameView.getCenter().x * 0.15, this->gameView.getCenter().y * 0.15);
    this->minimapCamera.setOrigin(this->minimapCamera.getSize().x / 2, this->minimapCamera.getSize().y / 2);

    this->multiplyUnits();
    this->updateUnits(mousePressedLeft, mousePressedRight, pressedKeys, realisedKeys, dt);

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

    bool renderPath = false;

    if (!this->buttons["Prepare"]->isActiv())
    {
        renderPath = true;
    }

    for (auto blood : this->bloods)
    {
        sf::Sprite sprite;

        sprite.setOrigin(this->textures["blood"].getSize().x / 2, this->textures["blood"].getSize().y / 2);
        sprite.setTexture(this->textures["blood"]);
        sprite.setPosition(blood.first.x, blood.first.y);
        sprite.setRotation(blood.second);
        sprite.setScale(0.5, 0.5);

        this->window->draw(sprite);
    }

    for (auto dead : this->deads)
    {
        sf::Sprite sprite;

        sprite.setOrigin(this->textures["dead"].getSize().x / 2, this->textures["dead"].getSize().y / 2);
        sprite.setTexture(this->textures["dead"]);
        sprite.setPosition(dead.first.x, dead.first.y);
        sprite.setRotation(dead.second);

        this->window->draw(sprite);
    }
    
    for (auto& unit : this->units)
    {
        unit->renderGame(this->gameView, renderPath);
    }

    for (auto& unit : this->units)
    {
        unit->renderArrow();
    }

    if (this->multiply)
    {
        this->window->draw(this->multiplyShape);
    }
    
    this->tilemap->renderMini(this->minimap);

    this->window->setView(this->minimap);
    this->window->draw(this->minimapCamera);
    this->window->setView(this->gameView);

    for (auto& unit : this->units)
    {
        unit->renderMini(this->minimap);
    }
    
    if (buttons.at("Prepare")->isActiv())
    {
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
            buttons.at("Prepare")->render();
        }
    }
    else
    {
        buttons.at("Prepare")->render();
    }
}