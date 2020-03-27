// Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Timber.h"

int main()
{
	Timber m_Timber; // main part of the game with all sprites
	bool paused = true;
	bool acceptInput = false; // Control the player input

	while( m_Timber.m_pWindow->isOpen() )
	{
		Event event;
		while (m_Timber.m_pWindow->pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true; // Listen for key presses again

				m_Timber.m_pAxe->setPosition(2000, m_Timber.m_pAxe->getPosition().y); // hide the axe
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) m_Timber.m_pWindow->close(); // exit the game

		if (Keyboard::isKeyPressed(Keyboard::Return)) // start game
		{
			paused = false;
			acceptInput = true;

			m_Timber.Start();
		}

		if (acceptInput && (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Left))) // Make sure we are accepting input
		{
			side playerSide = (Keyboard::isKeyPressed(Keyboard::Right)) ? side::RIGHT : side::LEFT;
			acceptInput = false;

			m_Timber.CutTree(playerSide);
		}

		if (!paused) m_Timber.UpdateSprites(paused, acceptInput); // Update the scene

		m_Timber.Draw(paused); // Draw the scene

		m_Timber.m_pPlayer->waitIfPlaying();
	}

    return 0;
}