#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    srand(time(0));
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 3, 8);
    this->camera = new Camera(this->window, 16000, 16000);
    this->gameView = this->window->getView();
    this->minimap.setViewport(sf::FloatRect(0.86f, 0, 0.15f, 0.25f));

    for (int i = 0; i < 10; i++)
    {
        this->units.push_back(new BaseUnit(this->window, this->tilemap, i, i, this->tilemap->mapUnits));
    }
}

GameState::~GameState()
{
    delete this->tilemap;
    
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

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll)
{
    this->updateMouse();
    this->camera->update(this->mousePosition, mouseScroll, sizeMapX, sizeMapY);
    this->gameView = this->window->getView();

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
    
    this->tilemap->renderMini(this->minimap);

    for (auto& unit : this->units)
    {
        unit->renderMini(this->minimap);
    }
}