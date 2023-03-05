#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    this->tilemap = new Tilemap(this->window, this->textures, 100, 100, 4, 16);
    this->baseUnit = new BaseUnit(this->window);
}

GameState::~GameState()
{
    delete this->tilemap;
    delete this->baseUnit;
}

void GameState::update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
    this->baseUnit->update(mousePressedLeft, pressedKeys, realisedKeys);

    if (this->baseUnit->isMoving())
    {
        this->baseUnit->moveTo();
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
    this->baseUnit->render();
}