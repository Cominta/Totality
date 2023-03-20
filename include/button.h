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

        bool activ;

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
        bool isActiv() const {return this->activ;}
        void setActiv(bool _activ) {this->activ = _activ;}
        void setTexture(sf::Texture* idle, sf::Texture* hover = nullptr, sf::Texture* active = nullptr);
        void setPosition(float x, float y) {this->sprite->setPosition(x, y);};
        void setPosition(sf::Vector2f pos) {this->sprite->setPosition(pos);};
        void setScale(float scaleX, float scaleY) {this->sprite->setScale(scaleX, scaleY);};
        sf::Vector2f getSize() {this->sprite->getTexture()->getSize();};
        void updateSprite() {this->sprite->setTexture(*this->idle);}
        void setOrigin(float x, float y) {this->sprite->setOrigin(x, y);}

        void update(sf::Vector2f mousePosition, bool mousePressed);
        void render();
};

#endif