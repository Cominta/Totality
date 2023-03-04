#include "state.h"

State::State(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : type(type), window(window), states(states), textures(textures)
{
    
}

State::~State() {}

bool State::find(std::vector<int> keys, int item)
{
    for (int i = 0; i < keys.size(); i++)
    {
        if (keys[i] == item)
        {
            return true;
        }
    }

    return false;
}

void State::updateMouse()
{   
    // this->mousePosition = sf::Mouse::getPosition(*(this->window));
    this->mousePosition = this->window->mapPixelToCoords(sf::Mouse::getPosition(*(this->window)));
}

void State::update()
{
    
}