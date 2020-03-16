#pragma once

#include <iostream>
#include <sstream>
#include "Animate.h"
#include "Message.h"

// constants
const int CLOUDS = 3;
const int XRES = 1920;
const int YRES = 1080;
const float TIME = 6.f;
const float TIMEBARWIDTH = 400.f;
const float TIMEBARHEIGHT = 80.f;
const float LOGSPEEDY = -1500.f;
const int NUM_BRANCHES = 6;
const float AXE_POSITION_LEFT = 700;
const float AXE_POSITION_RIGHT = 1075;

// map of animate class
typedef map<int, Animate*> Animap;
typedef map<int, Animate*>::iterator AniIt;

enum class side { LEFT, RIGHT, NONE };

class Timber
{
public:
	Timber();
	void initialiseSprites();
	void initialiseMessages();
	void initialiseTime();
	void Start();
	void CutTree(side aSide);
	void Draw(bool aPaused);
	void UpdateSprites(bool& aPaused, bool& aAcceptInput);
	void updateBranches(int seed);
	void hideBranches();
	Animate* m_pPlayer;
	Animate* m_pBG;
	Animate* m_pTree;
	Animate* m_pBee;
	Animate* m_pRIP;
	Animate* m_pAxe;
	Animate* m_pLog;

	Message* m_pMessage;
	Message* m_pScore;
	Message* m_pFPS;

	Sound m_Alarm;

	Sprite m_Branches[NUM_BRANCHES];
	side m_BranchPositions[NUM_BRANCHES];

	RectangleShape m_TimeBar;

	Animap m_mClouds;
	RenderWindow* m_pWindow;

	float m_LogSpeedX{ 0.0f };

private:
	float m_TimeBarDelta{ 0.0f };
	int m_Score{ 0 };
	float m_TimeRemaining{ TIME };
	side m_PlayerSide{ side::LEFT };
	Texture m_BranchTexture;

	Clock m_Clock;
	Time m_dt;
};

