#pragma once 

#include "state.h"
#include "tilemap.h"
#include "BaseUnit.h"

class GameState : public State 
{
    private:
        Tilemap* tilemap;
        BaseUnit* baseUnit;

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~GameState();

        void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
        void render() override;
};