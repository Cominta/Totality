#pragma once 

#include "state.h"
#include "button.h"
#include "tilemap.h"
#include "wariorUnit.h"
#include "archerUnit.h"
#include "viewMap.h"
#include "randomNums.h"
#include <thread>
#include <windows.h>
#include <chrono>
#include <future>

using namespace std::chrono_literals;

struct LoadingScreen
{
    sf::RectangleShape backgroundMain;
    sf::RectangleShape backLoadingBar;
    sf::RectangleShape loadingBar;
    sf::Text title;
}; 

class GameState : public State 
{
    private:
        const int sizeMapX;
        const int sizeMapY;

        LoadingScreen loadStruct;
        std::future<void> future;
        std::future_status status;
        const int maxWidthBar;
        float percentDone;
        sf::Font font;

        Tilemap* tilemap;
        Camera* camera;
        sf::View gameView;
        sf::View minimap;
        sf::RectangleShape minimapCamera;
        std::vector<BaseUnit*> units;
        std::vector<std::pair<sf::Vector2f, int>> bloods;
        std::vector<std::pair<sf::Vector2f, int>> deads;
        std::map<std::string, Button*> buttons;

        sf::RectangleShape multiplyShape;
        sf::Vector2f mStartPos;
        bool multiply;

        void setup(unsigned int seed);
        void loadingScreen();
        void generateBlood(std::pair<sf::Vector2f, int>& pos);
        void updateUnits(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, float dt);
        int updateButtons(bool mousePressedLeft);
        void multiplyUnits();

        static int seed;

    public:
        GameState(typeState type, sf::RenderWindow* window, std::stack<State*>& states, std::map<std::string, sf::Texture>& textures, unsigned int seed);
        ~GameState();

        void update(bool mousePressedLeft, bool mousePressedRight, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, int mouseScroll, float dt) override;
        void render() override;
};