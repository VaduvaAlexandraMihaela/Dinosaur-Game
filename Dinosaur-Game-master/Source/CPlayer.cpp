//-----------------------------------------------------------------------------
// File: CPlayer.cpp
//
// Desc: This file stores the player object class. This class performs tasks
//       such as player movement, some minor physics as well as rendering.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CPlayer Specific Includes
//-----------------------------------------------------------------------------
#include "CPlayer.h"

//-----------------------------------------------------------------------------
// Name : CPlayer () (Constructor)
// Desc : CPlayer Class Constructor
//-----------------------------------------------------------------------------
CPlayer::CPlayer(const BackBuffer *pBackBuffer, int depl, int xscore) 
{
	//m_pSprite = new Sprite("data/planeimg.bmp", "data/planemask.bmp");
	if(depl==6)
		m_pSprite = new Sprite("data/dino.bmp", RGB(0xff,0x00, 0xff));
	else
		if(depl==4)
		m_pSprite = new Sprite("data/dino2.bmp", RGB(0xff, 0x00, 0xff));
		else
				m_pSprite = new Sprite("data/dino420.bmp", RGB(0xff, 0x00, 0xff));

	m_pSprite->setBackBuffer( pBackBuffer );
	m_eSpeedState = SPEED_STOP;
	m_fTimer = 0;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 128;
	r.bottom = 128;

	score = new CScore(pBackBuffer, depl);
	scorey = xscore;

	for (int i = 0; i < 10; i++)
	{
		lives.push_front(new CLife(pBackBuffer, i * 50 + 20 + depl));
	}

	alive = true;

	isInvincible = false;

}

//-----------------------------------------------------------------------------
// Name : ~CPlayer () (Destructor)
// Desc : CPlayer Class Destructor
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	delete m_pSprite;
}

void CPlayer::Update(float dt)
{
	// Update sprite
	m_pSprite->update(dt);


	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommanded to be used in complex projects.

	// update internal time counter used in sound handling (not to overlap sounds)
	m_fTimer += dt;

	// A FSM is used for sound manager 
	switch(m_eSpeedState)
	{
	case SPEED_STOP:
		if(v > 35.0f)
		{
			m_eSpeedState = SPEED_START;
			//PlaySound("data/jet-start.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		break;
	case SPEED_START:
		if(v < 25.0f)
		{
			m_eSpeedState = SPEED_STOP;
			//PlaySound("data/jet-stop.wav", NULL, SND_FILENAME | SND_ASYNC);
			m_fTimer = 0;
		}
		else
			if(m_fTimer > 1.f)
			{
				//PlaySound("data/jet-cabin.wav", NULL, SND_FILENAME | SND_ASYNC);
				m_fTimer = 0;
			}
		break;
	}

	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see msdn reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void CPlayer::Draw()
{

		m_pSprite->draw();

		if (alive)
		{
	
			for (unsigned int i = 0; i < lives.size(); i++)
				lives[i]->Draw();

			score->Draw();
		}

}

void CPlayer::Reset()
{
	movementCoef = 1;
	isInvincible = false;
}

void CPlayer::newLife(const BackBuffer *pBackBuffer)
{
	setLives(pBackBuffer, 3);
}

void CPlayer::Invincible()
{
	isInvincible = true;
}

void CPlayer::Move(ULONG ulDirection)
{
	//stanga
	if (m_pSprite->mPosition.x < m_pSprite->width() - m_pSprite->mPosition.x + 20) {

		m_pSprite->mPosition.x = m_pSprite->width() - m_pSprite->mPosition.x + 20;
		m_pSprite->mVelocity.x = 0;

	}
	 //dreapta
	if (m_pSprite->mPosition.x > GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() /2 - 20) {
		m_pSprite->mPosition.x = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() /2 - 20;
		m_pSprite->mVelocity.x = 0;
	}
	//sus
	if (m_pSprite->mPosition.y < m_pSprite->height() - m_pSprite->mPosition.y) {
		m_pSprite->mPosition.y = m_pSprite->height() - m_pSprite->mPosition.y;
		m_pSprite->mVelocity.y = 0;
	}
	//jos
	if (m_pSprite->mPosition.y > GetSystemMetrics(SM_CYSCREEN) - m_pSprite->height() ) {
		m_pSprite->mPosition.y = GetSystemMetrics(SM_CYSCREEN) - m_pSprite->height() ;
		m_pSprite->mVelocity.y = 0;
	}

	

	if( ulDirection & CPlayer::DIR_LEFT )
		m_pSprite->mVelocity.x -= 5.1;

	if( ulDirection & CPlayer::DIR_RIGHT )
		m_pSprite->mVelocity.x += 5.1;

	if( ulDirection & CPlayer::DIR_FORWARD )
		m_pSprite->mVelocity.y -= 5.1;

	if( ulDirection & CPlayer::DIR_BACKWARD )
		m_pSprite->mVelocity.y += 5.1;
}

Vec2& CPlayer::Position()
{
	return m_pSprite->mPosition;

}


Vec2& CPlayer::Velocity()
{
	return m_pSprite->mVelocity;
}

void CPlayer::deleteLife()
{
	if (lives.size())
	{
		delete lives[0];
		lives.erase(lives.begin());
	}
	else
	{
		alive = false;
	}
}

void CPlayer::addLife(const BackBuffer *pBackBuffer)
{
	if (lives.size())
	{
		int liv = lives.size();
		lives.push_front(new CLife(pBackBuffer, liv * 50 ));
	}
	
}

void CPlayer::setLives(const BackBuffer *pBackBuffer, int i)
{

	if (i > lives.size())
	{
		for (int liv = lives.size(); liv < i; liv++)
		{
			lives.push_front(new CLife(pBackBuffer, liv * 40 ));
		}
	}
	else if (i < lives.size())
	{
		for (int liv = i; liv < lives.size(); liv++)
		{
			deleteLife();
		}
	}

}

int CPlayer::GetLives()
{
	return mLives;
}