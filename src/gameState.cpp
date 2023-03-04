#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures)
{
    this->tilemap = new Tilemap(this->window, this->textures, 1000, 1000, 1, 16);
}

GameState::~GameState()
{
    delete this->tilemap;
}

void GameState::update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
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
}