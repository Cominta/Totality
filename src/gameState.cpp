#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
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

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll)
{
    this->updateMouse();
    this->camera->update(this->mousePosition, mouseScroll, sizeMapX, sizeMapY);
    this->gameView = this->window->getView();

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

        this->units[i]->update(mousePressedLeft, mousePressedRight, realisedKeys, pressedKeys, this->units);
        this->units[i]->moveTo();
    }

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