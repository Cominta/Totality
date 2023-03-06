
#include "BaseUnit.h"

void BaseUnit::update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys)
{
	if (mousePressed)
	{
		if (mousePressed)
		{
			sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
			this->window->mapCoordsToPixel(sf::Vector2f(mousepos), this->window->getView());
			if (unit.getGlobalBounds().contains(sf::Vector2f(mousepos)))
			{
				this->b_active = true;
				this->setOutlineColor(255, 0 ,0);
			}
			else if (!find(pressedKeys, sf::Keyboard::LShift) || !find(pressedKeys, sf::Keyboard::RShift))
			{
				this->b_active = false;
				this->setOutlineColor(0, 0, 0);
			}
		}
	}
	else if (find(realisedKeys, sf::Keyboard::Enter))
	{
		if (isActiv())
		{
			sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
			this->window->mapCoordsToPixel(sf::Vector2f(mousepos), this->window->getView());
			this->setMove(mousepos);
			this->setIsMoving(true);
			this->speed.x = 10;
			this->speed.y = 10;
		}
	}

	this->unit.setPosition(this->unit.getPosition());
}

void BaseUnit::moveTo(std::vector<std::vector<int>> mapUnits)
{
	if (!this->b_moving)
	{
		return;
	}

	bool endX = false;
	bool endY = false;

	
	if (endX && endY)
	{
		b_moving = false;
		speed.x = 10;
		speed.y = 10;
	}
}

void BaseUnit::render()
{
	this->window->draw(this->unit);
}
