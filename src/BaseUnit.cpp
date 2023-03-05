
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
	this->hitbox->update();
}

void BaseUnit::moveTo(std::vector<BaseUnit*> foundRange)
{
	bool endX = false;
	bool endY = false;

	if (foundRange.size() >= 2)
	{
		for (auto unitFound : foundRange)
		{
			sf::Vector2f pos = pos;
			sf::Vector2f ourPos = this->unit.getPosition();
			HitboxSquare* hitboxOther = unitFound->getHitbox();
			this->b_moving = false;
			this->wayEnd.x = ourPos.x;
			this->wayEnd.y = ourPos.y;
			return;
		}	
	}

	if (unit.getPosition().x != this->wayEnd.x)
	{
		if (unit.getPosition().x > this->wayEnd.x)
		{
			if (unit.getPosition().x + (speed.x * -1) < this->wayEnd.x)
			{
				speed.x = unit.getPosition().x - this->wayEnd.x;
			}
			unit.move(speed.x * -1, 0);
		}
		else
		{
			if (unit.getPosition().x + speed.x > this->wayEnd.x)
			{
				speed.x = this->wayEnd.x - unit.getPosition().x;
			}
			unit.move(speed.x, 0);
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
			if (unit.getPosition().y + (speed.y * -1) < this->wayEnd.y)
			{
				speed.y = unit.getPosition().y - this->wayEnd.y;
			}
			unit.move(0, speed.y * -1);
		}
		else
		{
			if (unit.getPosition().y + speed.y > this->wayEnd.y)
			{
				speed.y = this->wayEnd.y - unit.getPosition().y;
			}
			unit.move(0, speed.y);
		}
	}
	else
	{
		endY = true;
	}
	if (endX && endY)
	{
		b_moving = false;
		speed.x = 10;
		speed.y = 10;
	}
}

void BaseUnit::getNextPos(HitboxSquare& box)
{
	if (!this->b_moving)
	{
		box = *this->hitbox;
		return;
	}

	box.setSize(this->hitbox->width, this->hitbox->height);

	int xPos = this->hitbox->x;
	int yPos = this->hitbox->y;

	if (unit.getPosition().y > this->wayEnd.y)
	{
		yPos -= this->speed.y;
	}

	else 
	{
		yPos += this->speed.y;
	}

	if (unit.getPosition().x > this->wayEnd.x)
	{
		xPos -= this->speed.x;
	}

	else 
	{
		xPos += this->speed.x;
	}

	box.setPosition(xPos, yPos);
}

void BaseUnit::render()
{
	this->window->draw(this->unit);
	this->hitbox->render();
}
