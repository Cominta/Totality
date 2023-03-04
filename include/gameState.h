#pragma once 

#include "state.h"
#include "tilemap.h"

class GameState : public State 
{
    private:
        Tilemap* tilemap;

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~GameState();

        void update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
        void render() override;
};