#pragma once

#include "state.h"

class MainState : public State
{
    public:
        MainState(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~MainState();

        void update();
        void render();
};