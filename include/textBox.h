#pragma once 

#include <iostream>
#include "SFML/Graphics.hpp"

class TextBox 
{
    private:
        sf::RenderWindow* window;
        sf::RectangleShape background;
        sf::Text text;
        sf::Font font;
        sf::Color backgoundColor;
        sf::Color backgoundColorOut;
        std::string str;

        bool active;
        bool cursor;
        int maxSymbols;

        int x;
        int y;

        int width;
        int height;

        void isActive(bool mousePressedLeft, sf::Vector2f mousePosition);
        void deleteLastChar();
        void handle(int charTyped);

    public:
        TextBox(sf::RenderWindow* window, int width, int height, float x, float y, int maxSymbols);

        void typedOn(sf::Event sfEvent);
        void update(bool mousePressedLeft, sf::Vector2f mousePosition, bool backspace);
        void render();
        std::string getStr() {return this->str;}
        void setStr(std::string str) {this->str = str;}
};