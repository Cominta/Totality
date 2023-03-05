#include "gameState.h"

GameState::GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : State(type, window, states, textures), sizeMapX(100), sizeMapY(100)
{
    this->tilemap = new Tilemap(this->window, this->textures, this->sizeMapX, this->sizeMapY, 3, 16);

    for (int i = 0; i < 3; i++)
    {
        this->units.push_back(new BaseUnit(this->window));
        // this->quadtree->insert(this->units[i]);
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

void GameState::update(bool mousePressedLeft, bool MousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
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

    for (auto& unit : this->units)
    {
        unit->render();
    }
}