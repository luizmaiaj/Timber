// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "Animate.h"
#include "Message.h"

const int CLOUDS = 3;
const int XRES = 1920;
const int YRES = 1080;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch? 
// Left or Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	std::cout << "Hello World!\n";

	// Create a video mode object
	VideoMode vm(XRES, YRES);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Default);

	// Create a texture to hold a graphic on the GPU
	Animate bg("graphics/background.png", "", 0, 0);

	// Make a tree sprite
	Animate Tree("graphics/tree.png", "sound/chop.wav", 810, 0);

	// Prepare the bee
	Animate Bee("graphics/bee.png", "sound/bee.wav", 0, 800);

	// Create a animate group with a map
	map<int, Animate*> mapClouds;
	map<int, Animate*>::iterator it;

	int i = 0;
	for (i = 0; i < CLOUDS; i++)
	{
		Animate *pCloud = new Animate("graphics/cloud.png", "", 0.0f, 150.0f * (i + 1));

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
	timeBar.setPosition((XRES / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	bool paused = true;

	// Draw some text
	int score = 0;

	// Assign the actual message
	Message messageText("fonts/KOMIKAP_.ttf", 75, 0, 0, "Press Enter to start!");
	Message scoreText("fonts/KOMIKAP_.ttf", 100, 20, 20, "Score = 0");
	Message fpsText("fonts/KOMIKAP_.ttf", 75, 1700, 20, "000");

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);

	messageText.setPosition(XRES / 2.0f, YRES / 2.0f);

	// Prepare 5 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Animate Player("graphics/player.png", "sound/death.wav", 580, 720);

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/alarm.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while( window.isOpen() )
	{
		// score ++;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}

		}

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
			timeRemaining = 6;

			// Make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			// Move the player into position
			Player.setPosition(580, 720);

			acceptInput = true;
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

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2.f / score) + .15f;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				Player.setPosition(1200, 720);

				// update the branches
				updateBranches(score);

				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				Tree.play();
			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;

				score++;

				// Add to the amount of time remaining
				timeRemaining += (2.f / score) + .15f;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);


				Player.setPosition(580, 720);

				// update the branches
				updateBranches(score);

				// set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				Tree.play();
			}
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

				messageText.setPosition(XRES / 2.0f, YRES / 2.0f);

				outOfTime.play();

				Bee.stop();
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
				Bee.setPosition((Bee.getDirection() == -1) ? XRES : -80.f, height);
				Bee.setActive(true);
				Bee.play();
			}
			else // Move the bee
			{
				Bee.move(dt.asSeconds());

				// Has the bee reached the right hand edge of the screen?
				if (Bee.getPosition().x < -80 || Bee.getPosition().x > XRES)
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

			stringstream ss2;
			ss2 << (int) (1 / dt.asSeconds());
			fpsText.setString(ss2.str());

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{

				float height = i * 150.f;

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(600, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);

				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log				
			if (logActive)
			{

				spriteLog.setPosition(
					spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

				// Has the insect reached the right hand edge of the screen?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new cloud next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// death
				paused = true;
				acceptInput = false;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// hide the player
				Player.setPosition(2000, 660);

				// hide the axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);

				// Change the text of the message
				messageText.setString("SQUISHED!!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(XRES / 2.0f,
					YRES / 2.0f);

				// Play the death sound
				Player.play();

				Bee.stop();
			}

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

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) 
		{
			window.draw(branches[i]);
		}

		// Draw the tree
		window.draw(Tree);

		// Draw the player
		window.draw(Player);

		// Draw the axe
		window.draw(spriteAxe);

		// Draraw the flying log
		window.draw(spriteLog);

		// Draw the gravestone
		window.draw(spriteRIP);

		// Draw the insect
		window.draw(Bee);

		// Draw the score
		window.draw(scoreText);

		window.draw(fpsText);

		// Draw the timebar
		window.draw(timeBar);

		if (paused) window.draw(messageText);

		// Show everything we just drew
		window.display();

		Player.playAndWait();
	}

    return 0;
}

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
