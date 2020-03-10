// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "Animate.h"

int main()
{
	int iClouds = 3;	
	std::cout << "Hello World!\n";

	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Default);

	// Create a texture to hold a graphic on the GPU
	Animate bg("graphics/background.png", 0, 0);

	// Make a tree sprite
	Animate Tree("graphics/tree.png", 810, 0);

	// Prepare the bee
	Animate Bee("graphics/bee.png", 0, 800);

	// Create a animate group with a map
	map<int, Animate*> mapClouds;
	map<int, Animate*>::iterator it;

	int i = 0;
	for (i = 0; i < iClouds; i++)
	{
		Animate *pCloud = new Animate("graphics/cloud.png", 0.0f, 150.0f * (i + 1));

		mapClouds[i] = pCloud;
	}

	// Variables to control time itself
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	// Draw some text
	int score = 0;

	Text messageText;
	Text scoreText;

	// We need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			// Reset the time and the score
			score = 0;
			timeRemaining = 5;
		}

		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			}

			// Setup the bee
			if (!Bee.getActive())
			{
				// How fast is the bee
				srand((int)time(0) * 10);
				Bee.setSpeed((float)(rand() % 200) + 200);

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (float)(rand() % 500) + 500;

				Bee.setDirection(0);
				Bee.setPosition((Bee.getDirection() == -1) ? 1920.f : -80.f, height);
				Bee.setActive(true);
			}
			else // Move the bee
			{
				Bee.move(dt.asSeconds());

				// Has the bee reached the right hand edge of the screen?
				if (Bee.getPosition().x < -80 || Bee.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					Bee.setActive(false);
				}
			}

			// Loop through clouds
			i = 1;
			for (it = mapClouds.begin(); it != mapClouds.end(); it++)
			{
				auto* pCloud = it->second;

				if (!pCloud->getActive())
				{
					// How fast is the cloud
					srand((int)time(0) * (10 * i));
					pCloud->setSpeed((float)(rand() % 200));

					// How high is the cloud
					srand((int)time(0) * (10 * i));
					float height = (rand() % (150 * i)) - (i * 50.0f);
					pCloud->setDirection(i + 1);
					pCloud->setPosition((pCloud->getDirection() == -1) ? 2300.f : -300.f, height);
					pCloud->setActive(true);
				}
				else
				{
					pCloud->move(dt.asSeconds());

					// Has the cloud reached the right hand edge of the screen?
					if (pCloud->getPosition().x < -300 || pCloud->getPosition().x > 2300)
					{
						// Set it up ready to be a whole new cloud next frame
						pCloud->setActive(false);
					}
				}
				i++;
			}

			// update the score
			stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());
		} // end if !paused

		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(bg.getSprite());

		// Draw the clouds
		for (it = mapClouds.begin(); it != mapClouds.end(); it++)
		{
			Animate *spCloud = it->second;
			window.draw(spCloud->getSprite());
		}

		// Draw the tree
		window.draw(Tree);

		// Draw the insect
		window.draw(Bee);

		// Draw the score
		window.draw(scoreText);

		// Draw the timebar
		window.draw(timeBar);

		if (paused) window.draw(messageText);

		// Show everything we just drew
		window.display();
	}

    return 0;
}
