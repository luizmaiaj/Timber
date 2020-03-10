#pragma once

#include <SFML/Graphics.hpp>
#include <string.h>

using namespace sf;
using namespace std;

class Animate : public Sprite, Texture
{
public:
	Animate() {};
	Animate(string aTextureFile, float aPositionX, float aPositionY);
	void setActive(bool aActive);
	bool getActive();
	void setSpeed(float aSpeed);
	void move(float aSeconds);
	void setDirection(int aDirection);
	int getDirection();
	float getSpeed();
	Sprite getSprite();

private:
	bool m_active{ false };
	float m_speed{ 0.0f };
	int m_direction{ 1 };
};

