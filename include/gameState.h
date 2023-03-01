#pragma once 

#include "state.h"

class GameState : public State 
{
    public:
        GameState(sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~GameState();

        void update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
        void render() override;
};