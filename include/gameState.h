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
        std::vector<std::pair<sf::Vector2f, int>> bloods;

        void generateBlood(std::pair<sf::Vector2f, int>& pos);
        void updateUnits(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys);

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures, unsigned int seed);
        ~GameState();

        void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll) override;
        void render() override;
};