

#ifndef _CBONUS_H_
#define _CBONUS_H_


#include "Main.h"
#include "Sprite.h"


class CBonus
{
public:
	//-------------------------------------------------------------------------
	// Enumerators
	//-------------------------------------------------------------------------
	enum DIRECTION
	{
		DIR_FORWARD = 1,
		DIR_BACKWARD = 2,
		DIR_LEFT = 4,
		DIR_RIGHT = 8,

	};


	enum ESpeedStates
	{
		SPEED_START,
		SPEED_STOP
	};

	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	CBonus(const BackBuffer *pBackBuffer, int x);
	virtual ~CBonus();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt);
	void					Draw();
	void					MoveDown(ULONG ulDirection);
	void					MoveLeft(ULONG ulDirection);
	Vec2&					Position();
	Vec2&					Velocity();
	int Height() { return m_pSprite->height(); }
	int Width() { return m_pSprite->width(); }
	
private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;

};

#endif