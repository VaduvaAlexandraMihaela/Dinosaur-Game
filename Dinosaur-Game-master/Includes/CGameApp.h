//-----------------------------------------------------------------------------
// File: CGameApp.h
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _CGAMEAPP_H_
#define _CGAMEAPP_H_

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "CTimer.h"
#include "CPlayer.h"
#include "CBonus.h"
#include "CEnemy.h"
#include "Sprite.h"
#include "BackBuffer.h"
#include "ImageFile.h"


//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Main Class Declarations
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp (Class)
// Desc : Central game engine, initialises the game and handles core processes.
//-----------------------------------------------------------------------------
class CGameApp
{
public:
	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
			 CGameApp();
	virtual ~CGameApp();

	//-------------------------------------------------------------------------
	// Public Functions for This Class
	//-------------------------------------------------------------------------
	LRESULT	 DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
	bool		InitInstance( LPCTSTR lpCmdLine, int iCmdShow );
	int		 BeginGame( );
	bool		ShutDown( );
	
private:
	//-------------------------------------------------------------------------
	// Private Functions for This Class
	//-------------------------------------------------------------------------
	bool		BuildObjects	  ( int val);
	void		ReleaseObjects	( );
	void		FrameAdvance	  ( );
	bool		CreateDisplay	 ( );
	void		ChangeDevice	  ( );
	void		SetupGameState	(int val);
	void		AnimateObjects	( );
	void		DrawObjects	   ( );
	void		ProcessInput	  ( );
	void		ManageCollisions();
	bool		CheckCollision(CPlayer &obj1, CBonus &obj2);
	bool		CheckCollision2(CPlayer &obj1, CEnemy &obj2);

	//-------------------------------------------------------------------------
	// Private Static Functions For This Class
	//-------------------------------------------------------------------------
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//-------------------------------------------------------------------------
	// Private Variables For This Class
	//-------------------------------------------------------------------------
	CTimer				   m_Timer;			// Game timer
	ULONG				   m_LastFrameRate;	// Used for making sure we update only when fps changes.
	
	HWND					m_hWnd;			 // Main window HWND
	HICON				   m_hIcon;			// Window Icon
	HMENU				   m_hMenu;			// Window Menu
	
	bool					m_bActive;		  // Is the application active ?

	ULONG				   m_nViewX;		   // X Position of render viewport
	ULONG				   m_nViewY;		   // Y Position of render viewport
	ULONG				   m_nViewWidth;	   // Width of render viewport
	ULONG				   m_nViewHeight;	  // Height of render viewport

	POINT				    m_OldCursorPos;	 // Old cursor position for tracking
	HINSTANCE				m_hInstance;
	  
	CImageFile				m_imgBackground;

	BackBuffer*				m_pBBuffer;
	CPlayer*				m_pPlayer;
	
	CEnemy*					m_pEnemy;
	CEnemy*					m_pEnemy2;
	CEnemy*					m_pEnemy3;
	CEnemy*					m_pEnemy4;
	CEnemy*                 m_pEnemy5;
	CEnemy*                 m_pEnemy6;

	CBonus*					m_pBonus;
	CBonus*					m_pBonus2;
	CBonus*					m_pBonus3;
	CBonus*					m_pBonus4;
};

#endif // _CGAMEAPP_H_