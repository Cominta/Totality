#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
    private:
        sf::RenderWindow* window;
        sf::Sprite* sprite;

        sf::Texture* idle;
        sf::Texture* hover;
        sf::Texture* active;

        float x;
        float y;
        float scale;

    public:
        enum class states
        {
            IDLE,
            HOVER,
            ACTIVE
        };

        Button::states state;

        int size;

        Button(sf::RenderWindow* window, float x, float y, float scale, sf::Texture* idle, sf::Texture* hover = nullptr, sf::Texture* active = nullptr);
        ~Button();

        bool isHover(sf::Vector2f mousePosition);
        void setTexture(sf::Texture* idle, sf::Texture* hover, sf::Texture* active);
        void setPosition(float x, float y) {this->sprite->setPosition(x, y);};
        void setPosition(sf::Vector2f pos) {this->sprite->setPosition(pos);};
        void setScale(float scaleX, float scaleY) {this->sprite->setScale(scaleX, scaleY);};
        sf::Vector2f getSize() {this->sprite->getTexture()->getSize();};

        void update(sf::Vector2f mousePosition, bool mousePressed);
        void render();
};

#endif