// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Timber.h"

int main()
{
	Timber m_Timber;
	Clock m_Clock;

	bool paused = true;

	// The player starts on the left
	side playerSide = side::LEFT;

	// Control the player input
	bool acceptInput = false;

	while( m_Timber.m_pWindow->isOpen() )
	{
		// score ++;
		Event event;
		while (m_Timber.m_pWindow->pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				m_Timber.m_pAxe->setPosition(2000, m_Timber.m_pAxe->getPosition().y);
			}
		}

		/*
		****************************************
		Handle the players input
		****************************************
		*/
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			m_Timber.m_pWindow->close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			acceptInput = true;

			m_Timber.Start();
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			// More code here next...
			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;

				acceptInput = false;

				m_Timber.CutTree(playerSide);
			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;

				acceptInput = false;

				m_Timber.CutTree(playerSide);
			}
		}

		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused)
		{
			Time dt = m_Clock.restart(); // restart the clock and returns the time since it has the clock has been started

			m_Timber.UpdateSprites(paused, acceptInput, dt, playerSide);
		}

		/*
		****************************************
		Draw the scene
		****************************************
		*/
		m_Timber.Draw(paused);

		m_Timber.m_pPlayer->waitIfPlaying();
	}

    return 0;
}

// Function definition
void Timber::updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		m_BranchPositions[j] = m_BranchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		m_BranchPositions[0] = side::LEFT;
		break;

	case 1:
		m_BranchPositions[0] = side::RIGHT;
		break;

	default:
		m_BranchPositions[0] = side::NONE;
		break;
	}
}

void Timber::hideBranches()
{
	// Make all the branches disappear
	for (int i = 1; i < NUM_BRANCHES; i++)
	{
		m_BranchPositions[i] = side::NONE;
	}
}

Timber::Timber()
{
	initialiseSprites();

	initialiseMessages();

	initialiseTime();
}

void Timber::initialiseSprites()
{
	m_pWindow = new RenderWindow(VideoMode(XRES, YRES), "Timber!!!", Style::Default); // window

	m_pPlayer = new Animate("graphics/player.png", "sound/death.wav", 580, 720); // player

	m_pBG = new Animate("graphics/background.png", "sound/wind.wav", 0, 0); // background

	m_pTree = new Animate("graphics/tree.png", "sound/chop.wav", 810, 0); // tree

	m_pBee = new Animate("graphics/bee.png", "sound/bee.wav", 0, 800); // bee

	for (int i = 0; i < CLOUDS; i++) // clouds
	{
		Animate* pCloud = new Animate("graphics/cloud.png", "", 0.0f, 150.0f * (i + 1));

		m_mClouds[i] = pCloud;
	}

	m_pRIP = new Animate("graphics/rip.png", "", 600, 860); // gravestone

	m_pAxe = new Animate("graphics/axe.png", "", 700, 830); // axe

	m_pLog = new Animate("graphics/log.png", "", 810, 720); // log

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/alarm.wav");
	Sound m_Alarm;
	m_Alarm.setBuffer(ootBuffer);

	// Prepare 5 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		m_Branches[i].setTexture(textureBranch);
		m_Branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		m_Branches[i].setOrigin(220, 20);
	}
}

void Timber::initialiseMessages()
{
	m_pMessage = new Message("fonts/KOMIKAP_.ttf", 75, 0, 0, "Press Enter to start!");
	m_pScore = new Message("fonts/KOMIKAP_.ttf", 100, 20, 20, "Score = 0");
	m_pFPS = new Message("fonts/KOMIKAP_.ttf", 75, 1700, 20, "000");

	// Position the text
	FloatRect textRect = m_pMessage->getLocalBounds();

	m_pMessage->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	m_pMessage->setPosition(XRES / 2.0f, YRES / 2.0f);
}

void Timber::initialiseTime()
{
	m_TimeBar.setSize(Vector2f(TIMEBARWIDTH, TIMEBARHEIGHT));
	m_TimeBar.setFillColor(Color::Red);
	m_TimeBar.setPosition((XRES / 2) - TIMEBARWIDTH / 2, 980);

	m_TimeBarDelta = TIMEBARWIDTH / TIME;
}

void Timber::Start()
{
	hideBranches();

	// Make sure the gravestone is hidden
	m_pRIP->setPosition(675, 2000);

	// Move the player into position
	m_pPlayer->setPosition(580, 720);

	m_pBG->play();

	m_Score = 0;

	m_TimeRemaining = 6.f;
}

void Timber::CutTree(side aSide)
{
	m_pAxe->setPosition((aSide == side::RIGHT)? AXE_POSITION_RIGHT : AXE_POSITION_LEFT, m_pAxe->getPosition().y);

	m_pPlayer->setPosition((aSide == side::RIGHT) ? 1200.f : 580.F, 720.f);

	updateBranches(m_Score); // update the branches

	// set the log flying to the left
	m_pLog->setPosition(810, 720);
	m_LogSpeedX = (aSide == side::RIGHT) ? -5000.f : 5000.f;
	m_pLog->setActive(true);

	m_pTree->play(); // Play a chop sound

	m_Score++;

	// Add to the amount of time remaining
	m_TimeRemaining += (2.f / m_Score) + .15f;
}

void Timber::Draw(bool aPaused)
{
	// Clear everything from the last frame
	m_pWindow->clear();

	// Draw our game scene here
	m_pWindow->draw(m_pBG->getSprite());

	// Draw the clouds
	for (AniIt it = m_mClouds.begin(); it != m_mClouds.end(); it++)
	{
		Animate* spCloud = it->second;
		m_pWindow->draw(spCloud->getSprite());
	}

	// Draw the branches
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		m_pWindow->draw(m_Branches[i]);
	}

	// Draw the tree
	m_pWindow->draw(*m_pTree);

	// Draw the player
	m_pWindow->draw(*m_pPlayer);

	// Draw the axe
	m_pWindow->draw(*m_pAxe);

	// Draraw the flying log
	m_pWindow->draw(*m_pLog);

	// Draw the gravestone
	m_pWindow->draw(*m_pRIP);

	// Draw the insect
	m_pWindow->draw(*m_pBee);

	// Draw the score
	m_pWindow->draw(*m_pScore);

	m_pWindow->draw(*m_pFPS);

	// Draw the timebar
	m_pWindow->draw(m_TimeBar);

	if (aPaused) m_pWindow->draw(*m_pMessage);

	// Show everything we just drew
	m_pWindow->display();
}

void Timber::UpdateSprites(bool& aPaused, bool& aAcceptInput, Time& aDT, side aPlayerSide)
{
	m_TimeRemaining -= aDT.asSeconds(); // Subtract from the amount of time remaining

	m_TimeBar.setSize(Vector2f(m_TimeBarDelta * m_TimeRemaining, TIMEBARHEIGHT)); // size up the time bar

	if (m_TimeRemaining <= 0.0f)
	{
		// Pause the game
		aPaused = true;

		// Change the message shown to the player
		m_pMessage->setString("Out of time!!");

		//Reposition the text based on its new size
		FloatRect textRect = m_pMessage->getLocalBounds();
		m_pMessage->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

		m_pMessage->setPosition(XRES / 2.0f, YRES / 2.0f);

		m_Alarm.play();

		m_pBee->stop();
		m_pBG->stop();
	}

	// Setup the bee
	if (!m_pBee->getActive())
	{
		// How fast is the bee
		srand((int)time(0) * 10);
		m_pBee->setSpeed((float)(rand() % 200) + 200);

		// How high is the bee
		srand((int)time(0) * 10);
		float height = (float)(rand() % 500) + 500;

		m_pBee->setDirection(0);
		m_pBee->setPosition((m_pBee->getDirection() == -1) ? XRES : -80.f, height);
		m_pBee->setActive(true);
		m_pBee->play();
	}
	else // Move the bee
	{
		m_pBee->move(aDT.asSeconds());

		// Has the bee reached the right hand edge of the screen?
		if (m_pBee->getPosition().x < -80 || m_pBee->getPosition().x > XRES)
		{
			// Set it up ready to be a whole new cloud next frame
			m_pBee->setActive(false);
		}
	}

	// Loop through clouds
	int i = 1;
	for (AniIt it = m_mClouds.begin(); it != m_mClouds.end(); it++)
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
			pCloud->move(aDT.asSeconds());

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
	ss << "Score = " << m_Score;
	m_pScore->setString(ss.str());

	stringstream ss2;
	ss2 << (int)(1 / aDT.asSeconds());
	m_pFPS->setString(ss2.str());

	// update the branch sprites
	for (int i = 0; i < NUM_BRANCHES; i++)
	{

		float height = i * 150.f;

		if (m_BranchPositions[i] == side::LEFT)
		{
			// Move the sprite to the left side
			m_Branches[i].setPosition(600, height);
			// Flip the sprite round the other way
			m_Branches[i].setRotation(180);
		}
		else if (m_BranchPositions[i] == side::RIGHT)
		{
			// Move the sprite to the right side
			m_Branches[i].setPosition(1330, height);
			// Set the sprite rotation to normal
			m_Branches[i].setRotation(0);

		}
		else
		{
			// Hide the branch
			m_Branches[i].setPosition(3000, height);
		}
	}

	// Handle a flying log				
	if (m_pLog->getActive())
	{
		m_pLog->setPosition(
			m_pLog->getPosition().x + (m_LogSpeedX * aDT.asSeconds()),
			m_pLog->getPosition().y + (LOGSPEEDY * aDT.asSeconds()));

		// Has the insect reached the right hand edge of the screen?
		if (m_pLog->getPosition().x < -100 ||
			m_pLog->getPosition().x > 2000)
		{
			// Set it up ready to be a whole new cloud next frame
			m_pLog->setActive(false);
			m_pLog->setPosition(810, 720);
		}
	}

	// has the player been squished by a branch?
	if (m_BranchPositions[5] == aPlayerSide)
	{
		// death
		aPaused = true;
		aAcceptInput = false;

		// Draw the gravestone
		m_pRIP->setPosition(525, 760);

		// hide the player
		m_pPlayer->setPosition(2000, 660);

		// hide the axe
		m_pAxe->setPosition(2000, m_pAxe->getPosition().y);

		// Change the text of the message
		m_pMessage->setString("SQUISHED!!");

		// Center it on the screen
		FloatRect textRect = m_pMessage->getLocalBounds();

		m_pMessage->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

		m_pMessage->setPosition(XRES / 2.0f, YRES / 2.0f);

		// Play the death sound
		m_pPlayer->play();

		m_pBee->stop();
		m_pBG->stop();
	}
}
