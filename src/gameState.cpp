#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 2, 8);
    this->camera = new Camera(this->window, 16000, 16000);

    for (int i = 0; i < 10; i++)
    {
        this->units.push_back(new BaseUnit(this->window, this->tilemap, i, i, this->tilemap->mapUnits));
    }
}

GameState::~GameState()
{
    delete this->tilemap;
    
    for (auto& unit : this->units)
    {
        delete unit;
    }
}

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    this->updateMouse();
    this->camera->update(this->mousePosition, 2.0f);

    for (auto& unit : this->units)
    {
        unit->update(mousePressedLeft, mousePressedRight, pressedKeys, realisedKeys, this->tilemap->mapUnits, this->tilemap->map);
        unit->moveTo(this->tilemap->mapUnits);
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
    this->tilemap->render();

    for (auto& unit : this->units)
    {
        unit->render();
    }
}