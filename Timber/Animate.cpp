#include "Animate.h"

Animate::Animate(string aTextureFile, float aPositionX, float aPositionY)
{
	loadFromFile(aTextureFile);

	setTexture(*this);
	setPosition(aPositionX, aPositionY);

	setOrigin(getScale().x / 2, getScale().y / 2);
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
	if (aDirection == -1 || aDirection == 1)
	{
		m_direction = aDirection;
	}
	else
	{
		if (aDirection == 0) aDirection = 1;

		srand((int)time(0) * aDirection * 10);
		m_direction = ((int)rand()) % 2;
		m_direction = (m_direction == 0) ? -1 : m_direction;

		if (m_direction == 1)
			setScale(-1.f, 1.f);
		else
			setScale(1.f, 1.f);
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
