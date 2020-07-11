#include "CBonus.h"

CBonus::CBonus(const BackBuffer *pBackBuffer, int x)
{
	if (x == 2) {
		m_pSprite = new Sprite("data/poti.bmp", RGB(0xff, 0x00, 0xff));
		m_pSprite->setBackBuffer(pBackBuffer);
		m_eSpeedState = SPEED_STOP;
		m_fTimer = 0;
	}
	else 
		if (x == 4) {
			m_pSprite = new Sprite("data/banut.bmp", RGB(0xff, 0x00, 0xff));
			m_pSprite->setBackBuffer(pBackBuffer);
			m_eSpeedState = SPEED_STOP;
			m_fTimer = 0;
		}
		else {
		m_pSprite = new Sprite("data/poti.bmp", RGB(0xff, 0x00, 0xff));
		m_pSprite->setBackBuffer(pBackBuffer);
		m_eSpeedState = SPEED_STOP;
		m_fTimer = 0;
	}

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 50;
	r.bottom = 50;

}

CBonus::~CBonus()
{
	delete m_pSprite;

}

void CBonus::Update(float dt)
{
	// Update sprite
	m_pSprite->update(dt);


	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();
}

void CBonus::Draw()
{
	m_pSprite->draw();
}


void CBonus::MoveLeft(ULONG ulDirection)
{

	if (ulDirection & CBonus::DIR_RIGHT)
		m_pSprite->mVelocity.x += 1.2;

	if (ulDirection & CBonus::DIR_LEFT)
		m_pSprite->mVelocity.x -= 1.2;


	//stanga
	/*if (m_pSprite->mPosition.x < m_pSprite->width() - m_pSprite->mPosition.x - 2500) {

		m_pSprite->mPosition.x = m_pSprite->width() - m_pSprite->mPosition.x - 2500;
		m_pSprite->mVelocity.x += 2000;

	}
	*/

	if (m_pSprite->mPosition.x > GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 + 2500) {
		m_pSprite->mPosition.x = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 + 2500;
		m_pSprite->mVelocity.x -= 2000;

	}

	if (m_pSprite->mPosition.x < GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 - 2500) {
		m_pSprite->mPosition.x = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 - 2500;
		m_pSprite->mVelocity.x += 1000;

	}
}
void CBonus::MoveDown(ULONG ulDirection) {
	if (ulDirection & CBonus::DIR_BACKWARD)
		m_pSprite->mVelocity.y += 0.2;

	if (ulDirection & CBonus::DIR_FORWARD)
		m_pSprite->mVelocity.y -= 0.2;

	if (m_pSprite->mPosition.y > GetSystemMetrics(SM_CXSCREEN) - m_pSprite->height() / 2 + 2500) {
		m_pSprite->mPosition.y = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->height() / 2 + 2500;
		m_pSprite->mVelocity.y -= 2000;

	}

	if (m_pSprite->mPosition.y < GetSystemMetrics(SM_CXSCREEN) - m_pSprite->height() / 2 - 2500) {
		m_pSprite->mPosition.y = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->height() / 2 - 2500;
		m_pSprite->mVelocity.y += 2000;

	}
}
Vec2& CBonus::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CBonus::Velocity()
{
	return m_pSprite->mVelocity;
}

