#include "button.h"

Button::Button(sf::RenderWindow* window, float x, float y, float scale, sf::Texture* idle, sf::Texture* hover, sf::Texture* active)
    : window(window), x(x), y(y), scale(scale)
{
    this->setTexture(idle, hover, active);

    this->sprite = new sf::Sprite();
    this->sprite->setTexture(*this->idle);
    this->sprite->setPosition(this->x, this->y);
    this->sprite->setScale(this->scale, this->scale);
    // this->sprite->setOrigin(this->sprite->getTexture()->getSize().x / 2, this->sprite->getTexture()->getSize().y / 2);

    this->state = Button::states::IDLE;

    this->size = 64;
}

Button::~Button()
{

}

bool Button::isHover(sf::Vector2f mousePosition)
{
    if (this->sprite->getGlobalBounds().contains(mousePosition.x, mousePosition.y))
    {
        return true;
    }

    return false;
}

void Button::setTexture(sf::Texture* idle, sf::Texture* hover, sf::Texture* active)
{
    this->idle = idle;
    this->hover = hover;
    this->active = active;
}

void Button::update(sf::Vector2f mousePosition, bool mousePressed)
{
    this->state = Button::states::IDLE;

    if (this->isHover(mousePosition))
    {
        if (mousePressed)
        {
            this->state = Button::states::ACTIVE;
        }

        else
        {
            this->state = Button::states::HOVER;
        }
    }

    else
    {
        this->state = Button::states::IDLE;
    }
}

void Button::render()
{
    this->window->draw(*this->sprite);
}