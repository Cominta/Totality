
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
	if (!this->b_moving)
	{
		return;
	}

	bool endX = false;
	bool endY = false;

	if (foundRange.size() >= 2)
	{
		for (auto unitFound : foundRange)
		{
			if (unitFound == this)
			{
				continue;
			}

			// if (!this->hitbox->intersects(unitFound->getHitbox()))
			// {
			// 	break;
			// }

			sf::Vector2f pos = unitFound->unit.getPosition();
			sf::Vector2f ourPos = this->unit.getPosition();
			HitboxSquare* hitboxOther = unitFound->getHitbox();

			if (unitFound->unit.getGlobalBounds().contains(this->wayEnd.x, this->wayEnd.y))
			{
				this->wayEnd.x = ourPos.x;
				this->wayEnd.y = ourPos.y;
				this->b_moving = false;

				break;
			}

			int moveX = 0;
			int moveY = 0;

			// ourPos = this->unit.getPosition();

			if (ourPos.y <= pos.y)
			{
				this->unit.move(0, this->speed.y * -1);
			}

			else if (ourPos.y > pos.y)
			{
				this->unit.move(0, this->speed.y);
			}

			else if (ourPos.x <= pos.x)
			{
				this->unit.move(this->speed.x * -1, 0);
			}

			else if (ourPos.x > pos.x)
			{
				this->unit.move(this->speed.x, 0);
			}

			ourPos = this->unit.getPosition();
			unitFound->wayEnd.x = pos.x;
			unitFound->wayEnd.y = pos.y;
			unitFound->b_moving = false;
		}

		return;
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

void BaseUnit::render()
{
	this->window->draw(this->unit);
	this->hitbox->render();
}
