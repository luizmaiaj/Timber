#include "Animate.h"

Animate::Animate(string aTextureFile, float aPositionX, float aPositionY)
{
	loadFromFile(aTextureFile);

	setTexture(*this);
	setPosition(aPositionX, aPositionY);
}

void Animate::setActive(bool aActive)
{
	m_active = aActive;
}

bool Animate::getActive()
{
	return m_active;
}

void Animate::setSpeed(float aSpeed)
{
	m_speed = aSpeed;
}

void Animate::move(float aSeconds)
{
	setPosition( getPosition().x + (m_direction * (getSpeed() * aSeconds)), getPosition().y);
}

void Animate::setDirection(int aDirection)
{
	if (!aDirection)
	{
		srand((int)time(0));
		m_direction = ((int) rand()) % 2;
		m_direction = (m_direction == 0) ? -1 : m_direction;
	}
	else
	{
		m_direction = aDirection;
	}
}

int Animate::getDirection()
{
	return m_direction;
}

float Animate::getSpeed()
{
	return m_speed;
}

Sprite Animate::getSprite()
{
	return *this;
}
