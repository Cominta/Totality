#include "textBox.h"

TextBox::TextBox(sf::RenderWindow* window, int width, int height, float x, float y)
    : window(window), width(width), height(height), x(x), y(y)
{
    this->backgoundColor = sf::Color(31, 36, 40);
    this->backgoundColorOut = sf::Color(36, 41, 46);

    this->background.setSize(sf::Vector2f(width, height));
    this->background.setPosition(this->x, this->y);
    this->background.setOutlineColor(this->backgoundColorOut);
    this->background.setFillColor(this->backgoundColorOut);
    this->background.setOutlineThickness(2.0f);

    this->font.loadFromFile("resources/fonts/mainmenu.ttf");

    this->text.setFont(font);
    this->text.setCharacterSize(height);
    this->text.setStyle(sf::Text::Bold);
    this->text.setFillColor(sf::Color::White);
    this->text.setPosition(this->x, this->y - this->height / 6);

    this->str = "";
    this->active = false;
    this->cursor = false;
    this->maxSymbols = 9;
}

void TextBox::isActive(bool mousePressedLeft, sf::Vector2f mousePosition)
{
    if (mousePressedLeft && this->background.getGlobalBounds().contains(mousePosition))
    {
        this->active = true;
    }

    else if (mousePressedLeft && !this->background.getGlobalBounds().contains(mousePosition))
    {
        this->active = false;
    }
}

void TextBox::deleteLastChar()
{
    std::string newT = "";

    for (int i = 0; i < this->str.size() - 1; i++) 
    {
        newT += this->str[i];
    }

    this->str = newT;
}

void TextBox::handle(int charTyped)
{
    if (charTyped != 8 && charTyped != 13 && charTyped != 27) 
    {
        this->str += static_cast<char>(charTyped);
    }

	else if (charTyped == 8) 
    {
        if (this->str.size() > 0) 
        {
            this->deleteLastChar();
        }
    }
}

void TextBox::typedOn(sf::Event sfEvent)
{
    if (this->active)
    {
        int charTyped = sfEvent.text.unicode;

        if (charTyped < 128 && this->str.size() <= this->maxSymbols)
        {
            this->handle(charTyped);
        }

        else if (this->str.size() > this->maxSymbols && charTyped == 8) // delete key
        {
            this->deleteLastChar();
        }
    }
}

void TextBox::update(bool mousePressedLeft, sf::Vector2f mousePosition, bool backspace)
{
    this->isActive(mousePressedLeft, mousePosition);

    if (this->active)
    {
        this->background.setOutlineColor(sf::Color::Cyan);
    }

    else 
    {
        this->background.setOutlineColor(this->backgoundColorOut);
    }

    this->text.setString(this->str);
}

void TextBox::render()
{
    this->window->draw(this->background);
    this->window->draw(this->text);
}