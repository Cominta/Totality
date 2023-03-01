#pragma once

#include "state.h"
#include "button.h"

class MainState : public State
{
    private:
        std::map<std::string, Button*> buttons; 

    public:
        MainState(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~MainState();

        void update(bool mousePressed) override;
        void render() override; 
};