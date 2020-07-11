
#ifndef _CBULLET_H_
#define _CBULLET_H_

#include "Main.h"
#include "Sprite.h"

class CBullet
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
	CBullet(const BackBuffer *pBackBuffer);
	virtual ~CBullet();

	//-------------------------------------------------------------------------
	// Public Functions for This Class.
	//-------------------------------------------------------------------------
	void					Update(float dt);
	void					Draw();
	void					Fire(Vec2 m_CPlayerPosition);
	void					Move(ULONG ulDirection);
	Vec2&					Position();
	Vec2&					Velocity();
	Vec2&					Rotate();
	
private:
	//-------------------------------------------------------------------------
	// Private Variables for This Class.
	//-------------------------------------------------------------------------
	Sprite*					m_pSprite;
	ESpeedStates			m_eSpeedState;
	float					m_fTimer;


};

#endif 