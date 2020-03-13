#include "Animate.h"

Animate::Animate(string aTextureFile, string aSoundFile, float aPositionX, float aPositionY)
{
	loadFromFile(aTextureFile);

	setTexture(*this);
	setPosition(aPositionX, aPositionY);

	setOrigin(getScale().x / 2, getScale().y / 2);

	if (aSoundFile.length() > 0)
	{
		m_SoundBuffer.loadFromFile(aSoundFile);
		m_Sound.setBuffer(m_SoundBuffer);
	}
}

void Animate::setActive(bool aActive)
{
	m_bActive = aActive;
}

bool Animate::getActive()
{
	return m_bActive;
}

void Animate::setSpeed(float aSpeed)
{
	m_fSpeed = aSpeed;
}

void Animate::move(float aSeconds)
{
	setPosition( getPosition().x + (m_iDirection * (getSpeed() * aSeconds)), getPosition().y);
}

void Animate::setDirection(int aDirection)
{
	if (aDirection == -1 || aDirection == 1)
	{
		m_iDirection = aDirection;
	}
	else
	{
		if (aDirection == 0) aDirection = 1;

		srand((int)time(0) * aDirection * 10);
		m_iDirection = ((int)rand()) % 2;
		m_iDirection = (m_iDirection == 0) ? -1 : m_iDirection;

		if (m_iDirection == 1)
			setScale(-1.f, 1.f);
		else
			setScale(1.f, 1.f);
	} 
}

void Animate::play()
{
	m_Sound.play();
}

void Animate::waitIfPlaying()
{
	while (m_Sound.getStatus() == Sound::Playing)
	{
		sleep(milliseconds(100));
	}
}

void Animate::stop()
{
	m_Sound.stop();
}

int Animate::getDirection()
{
	return m_iDirection;
}

float Animate::getSpeed()
{
	return m_fSpeed;
}

Sprite Animate::getSprite()
{
	return *this;
}
