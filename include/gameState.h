#pragma once 

#include "state.h"
#include "tilemap.h"
#include "BaseUnit.h"

class GameState : public State 
{
    private:
        const int sizeMapX;
        const int sizeMapY;

        Tilemap* tilemap;
        std::vector<BaseUnit*> units;

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~GameState();

        void update(bool mousePressedLeft, bool MousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys) override;
        void render() override;
};