//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//	   such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "Sprite.h"
#include "CLife.h"
#include "CScore.h"

//-----------------------------------------------------------------------------
// Main Class Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CPlayer (Class)
// Desc : Player class handles all player manipulation, update and management.
//-----------------------------------------------------------------------------
class CPlayer
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION 
	{ 
		DIR_FORWARD	 = 1, 
		DIR_BACKWARD	= 2, 
		DIR_LEFT		= 4, 
		DIR_RIGHT	   = 8, 
	
	};

	
	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 CPlayer(const BackBuffer *pBackBuffer,int depl,int score);
	virtual ~CPlayer();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update( float dt );
	void					Draw();
	void					Move(ULONG ulDirection);
	Vec2&					Position();
	Vec2&					Velocity();
	Vec2&					Rotate();
	CScore *			    score;
	int						scorey;
	int Height() { return m_pSprite->height(); }
	int Width() { return m_pSprite->width(); }
	void incrementScore(int i) { scorey += i; score->setScore(scorey); }
	void setScore(int i) { scorey = i; score->setScore(scorey); }
	int  getScore() { return scorey; }
	int  GetLives();
	void MoveFaster();
	void Invincible();
	void newLife(const BackBuffer *pBackBuffer);
	void Reset();
	//void SetLives(int newLives);
	int mLives;
	void deleteLife();
	bool isAlive() { return alive; }
	unsigned int NoOfLives() { return lives.size(); }
	void setLives(const BackBuffer *pBackBuffer, int i);
	void addLife(const BackBuffer *pBackBuffer);
private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;
	std::deque<CLife *>	    lives;
	bool					alive;
	int						movementCoef;
	bool					isInvincible;
	
};

#endif // _CPLAYER_H_