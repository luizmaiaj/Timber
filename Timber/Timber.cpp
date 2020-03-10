// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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
		Animate Cloud("graphics/cloud.png", 0.0f, 150.0f * (i + 1));

		mapClouds[i] = &Cloud;
	}

	// Variables to control time itself
	Clock clock;

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

		/*
		****************************************
		Update the scene
		****************************************
		*/
		// Measure time
		Time dt = clock.restart();

		// Setup the bee
		if (!Bee.getActive())
		{
			// How fast is the bee
			srand((int)time(0) * 10);
			Bee.setSpeed( (float) (rand() % 200) + 200 );

			// How high is the bee
			srand((int)time(0) * 10);
			float height = (float) (rand() % 500) + 500;
			Bee.setPosition(2000, height);
			Bee.setActive(true);

		}
		else // Move the bee
		{
			Bee.setPosition(
				Bee.getPosition().x -
				(Bee.getSpeed() * dt.asSeconds()),
				Bee.getPosition().y);

			// Has the bee reached the right hand edge of the screen?
			if (Bee.getPosition().x < -100)
			{
				// Set it up ready to be a whole new cloud next frame
				Bee.setActive(false);
			}
		}

		// Loop through clouds
		i = 1;
		for (it = mapClouds.begin(); it != mapClouds.end(); it++)
		{
			auto *pCloud = it->second;

			if (!pCloud->getActive())
			{
				// How fast is the cloud
				srand((int)time(0) * (10 * i));
				pCloud->setSpeed((float)(rand() % 200));

				// How high is the cloud
				srand((int)time(0) * (10 * i));
				float height = (float) ((rand() % (150 * i)) - (i>1)?150:0);
				pCloud->setPosition(-200, height);
				pCloud->setActive(true);
			}
			else
			{
				pCloud->setPosition(
					pCloud->getPosition().x +
					(pCloud->getSpeed() * dt.asSeconds()),
					pCloud->getPosition().y);

				// Has the cloud reached the right hand edge of the screen?
				if (pCloud->getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud next frame
					pCloud->setActive(false);
				}
			}
			i++;
		}

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
			Sprite spCloud = *(it->second);
			window.draw(spCloud);
		}

		// Draw the tree
		window.draw(Tree.getSprite());

		// Drawraw the insect
		window.draw(Bee.getSprite());

		// Show everything we just drew
		window.display();

	}

    return 0;
}
