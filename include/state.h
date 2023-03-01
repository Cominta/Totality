#pragma once

#include "SFML/Graphics.hpp"
#include <stack>
#include <map>

// абстрактный класс 
class State
{
    protected:
        sf::Vector2f mousePosition;
        sf::RenderWindow* window;
        std::stack<State*>& states;
        std::map<std::string, sf::Texture>& textures;

    public:
        State(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        void operator=(const State& obj) = delete;
        State(const State& obj) = delete;

        virtual ~State() = 0;

        void updateMouse();
        bool find(std::vector<int> keys, int item);

        virtual void update();
        virtual void update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) = 0;
        virtual void render() = 0;
};