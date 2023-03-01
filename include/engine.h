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

        bool mousePressed;

        void loadTextures();

    public:
        Engine(sf::RenderWindow* window);
        ~Engine();
        
        void updateSFML();
        void update();
        void render();

        void start();
};