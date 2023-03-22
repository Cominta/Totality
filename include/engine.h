#pragma once

#include "mainState.h"
#include <stack>

class Engine 
{
    private:
        sf::RenderWindow* window;
        sf::Event sfEvent;
        std::stack<State*> states;
        std::map<std::string, sf::Texture> textures;
        std::vector<int> pressedKeys;
        std::vector<int> realisedKeys;

        bool mousePressedLeft;
        bool mousePressedRight;
        int mouseScroll = 0;

        std::string typedString;

        sf::Clock clock;
        float dt;

        void loadTextures();
        void loadSounds();

    public:
        Engine(sf::RenderWindow* window);
        ~Engine();
        
        void updateDt();
        void updateSFML();
        void update();
        void render();

        void start();
};