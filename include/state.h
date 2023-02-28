#pragma once

#include "SFML/Graphics.hpp"
#include <stack>
#include <map>

// абстрактный класс 
class State
{
    protected:
        sf::RenderWindow* window;
        std::stack<State*>& states;
        std::map<std::string, sf::Texture>& textures;

    public:
        State(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        virtual ~State() = 0;

        virtual void update() = 0;
        virtual void render() = 0;
};