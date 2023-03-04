
#include "BaseUnit.h"

void BaseUnit::update(bool mousePressed, std::vector<int>& pressedKeys, std::vector<int>& realisedKeys, sf::Vector2f mousePosition)
{
	if (mousePressed)
	{
		if (find(pressedKeys, sf::Mouse::Button::Left))
		{
			if (unit.getGlobalBounds().contains(mousePosition))
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
			this->setMove(mousePosition);
			this->b_moving = true;
		}
	}
}

void BaseUnit::moveTo()
{
	bool endX = false;
	bool endY = false;

	if (unit.getPosition().x != this->wayEnd.x)
	{
		if (unit.getPosition().x > this->wayEnd.x)
		{
			unit.move(-1, 0);
		}
		else
		{
			unit.move(1, 0);
		}
	}
	else
	{
		endX = true;
	}
	if (unit.getPosition().y != this->wayEnd.y)
	{
		if (unit.getPosition().y > this->wayEnd.y)
		{
			unit.move(0, -1);
		}
		else
		{
			unit.move(0, 1);
		}
	}
	else
	{
		endY = true;
	}
	if (endX && endY)
	{
		b_moving = false;
	}
}

void BaseUnit::render()
{
	this->window->draw(this->unit);
}
