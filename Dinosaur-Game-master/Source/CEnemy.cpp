#include "CEnemy.h"

CEnemy::CEnemy(const BackBuffer *pBackBuffer,int x)
{
	if (x == 2) {
		m_pSprite = new Sprite("data/obstacle.bmp", RGB(0xff, 0x00, 0xff));
		m_pSprite->setBackBuffer(pBackBuffer);
		m_eSpeedState = SPEED_STOP;
		m_fTimer = 0;
	}
	else {
		if (x == 4) {
			m_pSprite = new Sprite("data/obstacle2.bmp", RGB(0xff, 0x00, 0xff));
			m_pSprite->setBackBuffer(pBackBuffer);
			m_eSpeedState = SPEED_STOP;
			m_fTimer = 0;
		}
		else {
			if (x == 3) {
				m_pSprite = new Sprite("data/obstacle1.bmp", RGB(0xff, 0x00, 0xff));
				m_pSprite->setBackBuffer(pBackBuffer);
				m_eSpeedState = SPEED_STOP;
				m_fTimer = 0;
			}
			else {
				if (x == 5)
				{
					m_pSprite = new Sprite("data/obstacle3.bmp", RGB(0xff, 0x00, 0xff));
					m_pSprite->setBackBuffer(pBackBuffer);
					m_eSpeedState = SPEED_STOP;
					m_fTimer = 0;
				}
				else {
					if (x == 6)
					{
						m_pSprite = new Sprite("data/obstacle4.bmp", RGB(0xff, 0x00, 0xff));
						m_pSprite->setBackBuffer(pBackBuffer);
						m_eSpeedState = SPEED_STOP;
						m_fTimer = 0;

					}
					else {
						if (x == 7)
						{
							m_pSprite = new Sprite("data/obstacle5.bmp", RGB(0xff, 0x00, 0xff));
							m_pSprite->setBackBuffer(pBackBuffer);
							m_eSpeedState = SPEED_STOP;
							m_fTimer = 0;
						}
					}
				}
			}
		}
	}

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 50;
	r.bottom = 50;

}

CEnemy::~CEnemy()
{
	delete m_pSprite;

}

void CEnemy::Update(float dt)
{
	// Update sprite
	m_pSprite->update(dt);


	// Get velocity
	double v = m_pSprite->mVelocity.Magnitude();
}

void CEnemy::Draw()
{
	m_pSprite->draw();
}

void CEnemy::MoveLeft(ULONG ulDirection)
{

	if (ulDirection & CEnemy::DIR_RIGHT)
		m_pSprite->mVelocity.x += 1.2;

	if (ulDirection & CEnemy::DIR_LEFT)
		m_pSprite->mVelocity.x -= 1.2;


	if (m_pSprite->mPosition.x > GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 + 2500) {
		m_pSprite->mPosition.x = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 + 2500;
		m_pSprite->mVelocity.x -= 2000;

	}

	if (m_pSprite->mPosition.x < GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 - 2500) {
		m_pSprite->mPosition.x = GetSystemMetrics(SM_CXSCREEN) - m_pSprite->width() / 2 - 2500;
		m_pSprite->mVelocity.x += 1000;

	}
}

Vec2& CEnemy::Position()
{
	return m_pSprite->mPosition;
}

Vec2& CEnemy::Velocity()
{
	return m_pSprite->mVelocity;
}

