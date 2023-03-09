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
        enum typeState 
        {
            MAINSTATE,
            GAMESTATE
        };

        typeState type;

        State(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        void operator=(const State& obj) = delete;
        State(const State& obj) = delete;

        virtual ~State() = 0;

        void updateMouse();
        bool find(std::vector<int> keys, int item);

        virtual void update();
        virtual void update(bool mousePressedLeft, bool MousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys);
        virtual void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll, float dt);
        virtual void render() = 0;
};