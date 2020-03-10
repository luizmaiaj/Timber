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

float Animate::getSpeed()
{
	return m_speed;
}

Sprite Animate::getSprite()
{
	return *this;
}
