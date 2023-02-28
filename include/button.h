#pragma once

#include "SFML/Graphics.hpp"

class Button
{
    private:
        sf::RenderWindow* window;

    public:
        Button();
        ~Button();

        void update();
        void render();
};