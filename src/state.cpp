#include "state.h"

State::State(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures)
    : window(window), states(states), textures(textures)
{
    
}

State::~State() {}