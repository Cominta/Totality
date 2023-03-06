#pragma once 

#include "state.h"
#include "tilemap.h"
#include "BaseUnit.h"
#include "viewMap.h"

class GameState : public State 
{
    private:
        const int sizeMapX;
        const int sizeMapY;

        Tilemap* tilemap;
        Camera* camera;
        sf::View gameView;
        sf::View minimap;
        std::vector<BaseUnit*> units;

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures);
        ~GameState();

        void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll) override;
        void render() override;
};