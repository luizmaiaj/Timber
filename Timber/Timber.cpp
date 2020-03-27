
#include "Timber.h"

// Function definition
void Timber::updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		m_BranchPositions[j] = m_BranchPositions[j - 1];
	}

	srand((int)time(0) + seed); // Spawn a new branch at position 0

	switch (rand() % 5)
	{
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

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/alarm.wav"); // Out of time
	Sound m_Alarm;
	m_Alarm.setBuffer(ootBuffer);

	m_BranchTexture.loadFromFile("graphics/branch.png"); // Prepare 5 branches

	for (int i = 0; i < NUM_BRANCHES; i++) // Set the texture for each branch sprite
	{
		m_Branches[i].setTexture(m_BranchTexture);
		m_Branches[i].setPosition(-2000, -2000);
		m_Branches[i].setOrigin(220, 20); // Set the sprite's origin to dead centre
	}
}

void Timber::initialiseMessages()
{
	m_pMessage = new Message("fonts/KOMIKAP_.ttf", 75, 0, 0, "Press Enter to start!");
	m_pScore = new Message("fonts/KOMIKAP_.ttf", 100, 20, 20, "Score = 0");
	m_pFPS = new Message("fonts/KOMIKAP_.ttf", 75, 1700, 20, "000");

	FloatRect textRect = m_pMessage->getLocalBounds(); // Position the text

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

	m_pRIP->setPosition(675, 2000); // Make sure the gravestone is hidden

	m_pPlayer->setPosition(580, 720); // Move the player into position

	m_pBG->play(true);

	m_Score = 0;

	m_TimeRemaining = 6.f;
}

void Timber::CutTree(side aSide)
{
	m_PlayerSide = aSide;

	m_pAxe->setPosition((m_PlayerSide == side::RIGHT) ? AXE_POSITION_RIGHT : AXE_POSITION_LEFT, m_pAxe->getPosition().y);

	m_pPlayer->setPosition((m_PlayerSide == side::RIGHT) ? 1200.f : 580.F, 720.f);

	updateBranches(m_Score); // update the branches

	m_pLog->setPosition(810, 720); // set the log flying to the left
	m_LogSpeedX = (m_PlayerSide == side::RIGHT) ? -5000.f : 5000.f;
	m_pLog->setActive(true);

	m_pTree->play(false); // Play a chop sound

	m_Score++;

	m_TimeRemaining += (2.f / m_Score) + .15f; // Add to the amount of time remaining
}

void Timber::Draw(bool aPaused)
{
	m_pWindow->clear(); // Clear everything from the last frame

	m_pWindow->draw(m_pBG->getSprite()); // Draw our game scene here

	for (AniIt it = m_mClouds.begin(); it != m_mClouds.end(); it++) // Draw the clouds
	{
		Animate* spCloud = it->second;
		m_pWindow->draw(spCloud->getSprite());
	}

	for (int i = 0; i < NUM_BRANCHES; i++) // Draw the branches
	{
		m_pWindow->draw(m_Branches[i]);
	}

	m_pWindow->draw(*m_pTree); // Draw the tree

	m_pWindow->draw(*m_pPlayer); // Draw the player

	m_pWindow->draw(*m_pAxe); // Draw the axe

	m_pWindow->draw(*m_pLog); // Draraw the flying log

	m_pWindow->draw(*m_pRIP); // Draw the gravestone

	m_pWindow->draw(*m_pBee); // Draw the insect

	m_pWindow->draw(*m_pScore); // Draw the score

	m_pWindow->draw(*m_pFPS); // draw the fps counters

	m_pWindow->draw(m_TimeBar); // Draw the timebar

	if (aPaused) m_pWindow->draw(*m_pMessage);

	m_pWindow->display(); // Show everything we just drew
}

void Timber::UpdateSprites(bool& aPaused, bool& aAcceptInput)
{
	m_dt = m_Clock.restart(); // restart the clock and returns the time since it has the clock has been started

	m_TimeRemaining -= m_dt.asSeconds(); // Subtract from the amount of time remaining

	m_TimeBar.setSize(Vector2f(m_TimeBarDelta * m_TimeRemaining, TIMEBARHEIGHT)); // size up the time bar

	if (m_TimeRemaining <= 0.0f)
	{
		aPaused = true; // Pause the game

		m_pMessage->setString("Out of time!!"); // Change the message shown to the player

		//Reposition the text based on its new size
		m_pMessage->setOrigin(m_pMessage->getLocalBounds().width / 2.0f, m_pMessage->getLocalBounds().height / 2.0f);

		m_pMessage->setPosition(XRES / 2.0f, YRES / 2.0f);

		m_Alarm.play();

		m_pBee->stop();
		m_pBG->stop();
	}

	if (!m_pBee->getActive()) // Setup the bee
	{
		srand((int)time(0) * 10); // How fast is the bee
		m_pBee->setSpeed((float)(rand() % 200) + 200);

		srand((int)time(0) * 10); // How high is the bee
		float height = (float)(rand() % 500) + 500;

		m_pBee->setDirection(0);
		m_pBee->setPosition((m_pBee->getDirection() == -1) ? XRES : -80.f, height);
		m_pBee->setActive(true);
		m_pBee->play(false);
	}
	else // Move the bee
	{
		m_pBee->move(m_dt.asSeconds());

		if (m_pBee->getPosition().x < -80 || m_pBee->getPosition().x > XRES) // Has the bee reached the right hand edge of the screen?
		{
			m_pBee->setActive(false);
		}
	}

	int i = 1;
	for (AniIt it = m_mClouds.begin(); it != m_mClouds.end(); it++) // Loop through clouds
	{
		auto* pCloud = it->second;

		if (!pCloud->getActive())
		{
			srand((int)time(0) * (10 * i)); // How fast is the cloud
			pCloud->setSpeed((float)(rand() % 200));

			srand((int)time(0) * (10 * i)); // How high is the cloud
			float height = (rand() % (150 * i)) - (i * 50.0f);
			pCloud->setDirection(i + 1);
			pCloud->setPosition((pCloud->getDirection() == -1) ? 2300.f : -300.f, height);
			pCloud->setActive(true);
		}
		else
		{
			pCloud->move(m_dt.asSeconds());

			if (pCloud->getPosition().x < -300 || pCloud->getPosition().x > 2300) // Has the cloud reached the right hand edge of the screen?
			{
				pCloud->setActive(false);
			}
		}
		i++;
	}

	stringstream ss;
	ss << "Score = " << m_Score; // update the score
	m_pScore->setString(ss.str());

	stringstream ss2;
	ss2 << (int)(1 / m_dt.asSeconds()); // update the fps counter
	m_pFPS->setString(ss2.str());

	for (int i = 0; i < NUM_BRANCHES; i++) // update the branch sprites
	{
		float height = i * 150.f;

		if (m_BranchPositions[i] == side::LEFT)
		{
			m_Branches[i].setPosition(600, height); // Move the sprite to the left side
			m_Branches[i].setRotation(180); // Flip the sprite round the other way
		}
		else if (m_BranchPositions[i] == side::RIGHT)
		{
			m_Branches[i].setPosition(1330, height); // Move the sprite to the right side
			m_Branches[i].setRotation(0); // Set the sprite rotation to normal
		}
		else
		{
			m_Branches[i].setPosition(3000, height); // Hide the branch
		}
	}

	if (m_pLog->getActive()) // Handle a flying log				
	{
		m_pLog->setPosition(m_pLog->getPosition().x + (m_LogSpeedX * m_dt.asSeconds()), m_pLog->getPosition().y + (LOGSPEEDY * m_dt.asSeconds()));

		if (m_pLog->getPosition().x < -100 || m_pLog->getPosition().x > 2000) // Has the insect reached the right hand edge of the screen?
		{
			m_pLog->setActive(false);
			m_pLog->setPosition(810, 720);
		}
	}

	if (m_BranchPositions[5] == m_PlayerSide) // has the player been squished by a branch?
	{
		aPaused = true;
		aAcceptInput = false;

		m_pRIP->setPosition(525, 760); // Draw the gravestone

		m_pPlayer->setPosition(2000, 660); // hide the player

		m_pAxe->setPosition(2000, m_pAxe->getPosition().y); // hide the axe

		m_pMessage->setString("SQUISHED!!"); // Change the text of the message

		FloatRect textRect = m_pMessage->getLocalBounds(); // Center it on the screen

		m_pMessage->setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

		m_pMessage->setPosition(XRES / 2.0f, YRES / 2.0f);

		m_pPlayer->play(false); // Play the death sound

		m_pBee->stop();
		m_pBG->stop();
	}
}