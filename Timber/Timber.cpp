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
		Animate *pCloud = new Animate("graphics/cloud.png", 0.0f, 150.0f * (i + 1));

		mapClouds[i] = pCloud;
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

			Bee.setDirection(0);
			Bee.setPosition((Bee.getDirection() == -1)? 1920.f : -80.f, height);
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
			auto *pCloud = it->second;

			if (!pCloud->getActive())
			{
				// How fast is the cloud
				srand((int)time(0) * (10 * i));
				pCloud->setSpeed((float)(rand() % 200));

				// How high is the cloud
				srand((int)time(0) * (10 * i));
				float height = (rand() % (150 * i)) - (i * 50.0f);
				pCloud->setDirection(i+1);
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

		// Drawraw the insect
		window.draw(Bee);

		// Show everything we just drew
		window.display();
	}

    return 0;
}
