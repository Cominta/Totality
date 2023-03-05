#include "hitboxSquare.h"

HitboxSquare::HitboxSquare(sf::RenderWindow* window, float width, float height, sf::CircleShape* parentShape)
    : Hitbox(window), parentShape(parentShape), width(width), height(height)
{
    float thickness = 1.0f;

    this->hitboxShape = new sf::RectangleShape(sf::Vector2f(width, height));
    this->hitboxShape->setOutlineColor(sf::Color::Magenta);
    this->hitboxShape->setFillColor(sf::Color::Transparent);
    this->hitboxShape->setOutlineThickness(thickness);
    this->hitboxShape->setOrigin(width / 2, height / 2);
}

HitboxSquare::~HitboxSquare()
{
    delete this->hitboxShape;
}

HitboxSquare& HitboxSquare::operator=(const HitboxSquare& obj)
{
    this->width = obj.width;
    this->height = obj.height;
    this->x = obj.x;
    this->y = obj.y;
    this->parentShape = nullptr;

    this->hitboxShape = new sf::RectangleShape(sf::Vector2f(width, height));
    this->hitboxShape->setOutlineColor(sf::Color::Magenta);
    this->hitboxShape->setFillColor(sf::Color::Transparent);
    this->hitboxShape->setOutlineThickness(1.0f);
    this->hitboxShape->setOrigin(width / 2, height / 2);

    this->hitboxShape->setPosition(obj.x, obj.y);
}

bool HitboxSquare::intersects(HitboxSquare* other)
{
    return this->hitboxShape->getGlobalBounds().intersects(other->getShape()->getGlobalBounds());
}

void HitboxSquare::update()
{
    this->x = this->parentShape->getPosition().x;
    this->y = this->parentShape->getPosition().y;

    if (this->parentShape != nullptr)
    {
        this->hitboxShape->setPosition(x, y);
        this->hitboxShape->setRotation(this->parentShape->getRotation());
    }
}

void HitboxSquare::render()
{
    this->window->draw(*this->hitboxShape);
}