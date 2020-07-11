//-----------------------------------------------------------------------------
// File: CGameApp.cpp
//
// Desc: Game Application class, this is the central hub for all app processing
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CGameApp Specific Includes
//-----------------------------------------------------------------------------
#include "CGameApp.h"
#include <dos.h>


extern HINSTANCE g_hInst;

//-----------------------------------------------------------------------------
// CGameApp Member Functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name : CGameApp () (Constructor)
// Desc : CGameApp Class Constructor
//-----------------------------------------------------------------------------
CGameApp::CGameApp()
{
	// Reset / Clear all required values
	m_hWnd			= NULL;
	m_hIcon			= NULL;
	m_hMenu			= NULL;
	m_pBBuffer		= NULL;
	m_pPlayer		= NULL;

	m_pEnemy		= NULL;
	m_pEnemy2		= NULL;
	m_pEnemy3		= NULL;
	m_pEnemy4		= NULL;
	m_pEnemy5		= NULL;
	m_pEnemy6		= NULL;

	m_pBonus		= NULL;
	m_pBonus2		= NULL;
	m_pBonus3		= NULL;
	m_pBonus4		= NULL;
	m_LastFrameRate = 0;
}

//-----------------------------------------------------------------------------
// Name : ~CGameApp () (Destructor)
// Desc : CGameApp Class Destructor
//-----------------------------------------------------------------------------
CGameApp::~CGameApp()
{
	// Shut the engine down
	ShutDown();
}

//-----------------------------------------------------------------------------
// Name : InitInstance ()
// Desc : Initialises the entire Engine here.
//-----------------------------------------------------------------------------
bool CGameApp::InitInstance( LPCTSTR lpCmdLine, int iCmdShow )
{
	// Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }

	// Build Objects
	if (!BuildObjects(7)) 
	{ 
		MessageBox( 0, _T("Failed to initialize properly. Reinstalling the application may solve this problem.\nIf the problem persists, please contact technical support."), _T("Fatal Error"), MB_OK | MB_ICONSTOP);
		ShutDown(); 
		return false; 
	}

	// Set up all required game states
	SetupGameState(7);

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : CreateDisplay ()
// Desc : Create the display windows, devices etc, ready for rendering.
//-----------------------------------------------------------------------------
bool CGameApp::CreateDisplay()
{
	HMONITOR hmon = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi = { sizeof(mi) };
	if (!GetMonitorInfo(hmon, &mi)) return NULL;

	LPTSTR			WindowTitle		= _T("GameFramework");
	LPCSTR			WindowClass		= _T("GameFramework_Class");
	USHORT			Width			= mi.rcMonitor.right;
	USHORT			Height			= mi.rcMonitor.bottom;
	RECT			rc;
	WNDCLASSEX		wcex;


	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= CGameApp::StaticWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInst;
	wcex.hIcon			= LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= WindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON));

	if(RegisterClassEx(&wcex)==0)
		return false;

	// Retrieve the final client size of the window
	::GetClientRect( m_hWnd, &rc );
	m_nViewX		= rc.left;
	m_nViewY		= rc.top;
	m_nViewWidth	= rc.right - rc.left;
	m_nViewHeight	= rc.bottom - rc.top;

	PlaySound("data/Dino_Megalovania.wav", NULL, SND_FILENAME | SND_ASYNC);

	m_hWnd = CreateWindow(WindowClass, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, NULL, NULL, g_hInst, this);

	if (!m_hWnd)
		return false;

	// Show the window
	ShowWindow(m_hWnd, SW_SHOW);

	// Success!!
	return true;
}

//-----------------------------------------------------------------------------
// Name : BeginGame ()
// Desc : Signals the beginning of the physical post-initialisation stage.
//		From here on, the game engine has control over processing.
//-----------------------------------------------------------------------------
int CGameApp::BeginGame()
{
	MSG		msg;

	// Start main loop
	while(true) 
	{
		// Did we recieve a message, or are we idling ?
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		} 
		else 
		{
			// Advance Game Frame.
			FrameAdvance();

		} // End If messages waiting
	
	} // Until quit message is receieved

	return 0;
}

//-----------------------------------------------------------------------------
// Name : ShutDown ()
// Desc : Shuts down the game engine, and frees up all resources.
//-----------------------------------------------------------------------------
bool CGameApp::ShutDown()
{
	// Release any previously built objects
	ReleaseObjects ( );
	
	// Destroy menu, it may not be attached
	if ( m_hMenu ) DestroyMenu( m_hMenu );
	m_hMenu		 = NULL;

	// Destroy the render window
	SetMenu( m_hWnd, NULL );
	if ( m_hWnd ) DestroyWindow( m_hWnd );
	m_hWnd		  = NULL;
	
	// Shutdown Success
	return true;
}

//-----------------------------------------------------------------------------
// Name : StaticWndProc () (Static Callback)
// Desc : This is the main messge pump for ALL display devices, it captures
//		the appropriate messages, and routes them through to the application
//		class for which it was intended, therefore giving full class access.
// Note : It is VITALLY important that you should pass your 'this' pointer to
//		the lpParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//-----------------------------------------------------------------------------
LRESULT CALLBACK CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if ( Message == WM_CREATE ) SetWindowLong( hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	// Obtain the correct destination for this message
	CGameApp *Destination = (CGameApp*)GetWindowLong( hWnd, GWL_USERDATA );
	
	// If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc( hWnd, Message, wParam, lParam );
	
	// No destination found, defer to system...
	return DefWindowProc( hWnd, Message, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name : DisplayWndProc ()
// Desc : The display devices internal WndProc function. All messages being
//		passed to this function are relative to the window it owns.
//-----------------------------------------------------------------------------
LRESULT CGameApp::DisplayWndProc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
	static UINT			fTimer;	
	

	// Determine message type
	switch (Message)
	{
		case WM_CREATE:
			break;
		
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		case WM_SIZE:
			if ( wParam == SIZE_MINIMIZED )
			{
				// App is inactive
				m_bActive = false;
			
			} // App has been minimized
			else
			{
				// App is active
				m_bActive = true;

				// Store new viewport sizes
				m_nViewWidth  = LOWORD( lParam );
				m_nViewHeight = HIWORD( lParam );
			
			} // End if !Minimized

			break;

		case WM_LBUTTONDOWN:
			// Capture the mouse
			SetCapture( m_hWnd );
			GetCursorPos( &m_OldCursorPos );
			break;

		case WM_LBUTTONUP:
			// Release the mouse
			ReleaseCapture( );
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		

			}
			break;


	

		case WM_COMMAND:
			break;

		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);

	} // End Message Switch
	
	return 0;
}

//-----------------------------------------------------------------------------
// Name : BuildObjects ()
// Desc : Build our demonstration meshes, and the objects that instance them
//-----------------------------------------------------------------------------
bool CGameApp::BuildObjects(int val)
{
	
	m_pBBuffer = new BackBuffer(m_hWnd, m_nViewWidth, m_nViewHeight);
	if (val == 7)
		m_pPlayer = new CPlayer(m_pBBuffer, 6, 0);
	else
		if (val == 9)
		{
			int x = m_pPlayer->getScore()+100;
			m_pPlayer = new CPlayer(m_pBBuffer, 4, x);
		}
		else {
			int y = m_pPlayer->getScore()+50;
			m_pPlayer = new CPlayer(m_pBBuffer, 5, y);
		}

	m_pEnemy = new CEnemy(m_pBBuffer, 2);
	m_pEnemy2 = new CEnemy(m_pBBuffer, 4);
	m_pEnemy3 = new CEnemy(m_pBBuffer, 3);
	m_pEnemy4 = new CEnemy(m_pBBuffer, 5);
	m_pEnemy5 = new CEnemy(m_pBBuffer, 6);
	m_pEnemy6 = new CEnemy(m_pBBuffer, 7);

	m_pBonus = new CBonus(m_pBBuffer, 2);
	m_pBonus2 = new CBonus(m_pBBuffer, 300);
	m_pBonus3 = new CBonus(m_pBBuffer, 4);
	m_pBonus4 = new CBonus(m_pBBuffer, 4);

	if(!m_imgBackground.LoadBitmapFromFile("data/back.bmp", GetDC(m_hWnd)))
		return false;

	// Success!
	return true;
}

//-----------------------------------------------------------------------------
// Name : SetupGameState ()
// Desc : Sets up all the initial states required by the game.
//-----------------------------------------------------------------------------
void CGameApp::SetupGameState(int val)
{	if(val==7)
		m_pPlayer->Position() = Vec2(100, 450);
	else if(val==9)
		m_pPlayer->Position() = Vec2(1400, 450);
		else
			m_pPlayer->Position() = Vec2(1400, 450);

	m_pEnemy->Position() = Vec2(1900, 450);
	m_pEnemy2->Position() = Vec2(-100, 450);
	m_pEnemy3->Position() = Vec2(-100, 200);
	m_pEnemy4->Position() = Vec2(1900, 700);
	m_pEnemy5->Position() = Vec2(-100, 700);
	m_pEnemy6->Position() = Vec2(1900, 350);

	m_pBonus->Position() = Vec2(1600, 250);
	m_pBonus2->Position() = Vec2(-400, 250);
	m_pBonus3->Position() = Vec2(1000, 500);
	m_pBonus4->Position() = Vec2(700, 500);
}

//-----------------------------------------------------------------------------
// Name : ReleaseObjects ()
// Desc : Releases our objects and their associated memory so that we can
//		rebuild them, if required, during our applications life-time.
//-----------------------------------------------------------------------------
void CGameApp::ReleaseObjects( )
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;

	}
	
	if (m_pBonus != NULL)
	{
		delete m_pBonus;
		m_pBonus = NULL;

	}

	if (m_pBonus2 != NULL)
	{
		delete m_pBonus2;
		m_pBonus2 = NULL;

	}
	if (m_pBonus3 != NULL)
	{
		delete m_pBonus3;
		m_pBonus3 = NULL;

	}

	if (m_pBonus4 != NULL)
	{
		delete m_pBonus4;
		m_pBonus4 = NULL;

	}

	if (m_pEnemy != NULL)
	{
		delete m_pEnemy;
		m_pEnemy = NULL;

	}

	if (m_pEnemy2 != NULL)
	{
		delete m_pEnemy2;
		m_pEnemy2 = NULL;

	}

	if (m_pEnemy3 != NULL)
	{
		delete m_pEnemy3;
		m_pEnemy3 = NULL;
	}

	if (m_pEnemy4 != NULL)
	{
		delete m_pEnemy4;
		m_pEnemy4 = NULL;
	}

	if (m_pEnemy5 != NULL)
	{
		delete m_pEnemy5;
		m_pEnemy5 = NULL;
	}

	if (m_pEnemy6 != NULL)
	{
		delete m_pEnemy6;
		m_pEnemy6 = NULL;
	}

	if (m_pBBuffer != NULL)
	{
		delete m_pBBuffer;
		m_pBBuffer = NULL;

	
	}
	
}
	

//-----------------------------------------------------------------------------
// Name : FrameAdvance () (Private)
// Desc : Called to signal that we are now rendering the next frame.
//-----------------------------------------------------------------------------
void CGameApp::FrameAdvance()
{
	static TCHAR FrameRate[ 500 ];
	static TCHAR TitleBuffer[ 255 ];
	
	// Advance the timer
	m_Timer.Tick( );

	// Skip if app is inactive
	if ( !m_bActive ) return;
	
	// Get / Display the framerate
	if ( m_LastFrameRate != m_Timer.GetFrameRate() )
	{
		m_LastFrameRate = m_Timer.GetFrameRate( FrameRate, 50 );
		sprintf_s( TitleBuffer, _T("Game : %s"), FrameRate );
		SetWindowText( m_hWnd, TitleBuffer );

	} // End if Frame Rate Altered
	// Skip if app is inactive
	if (!m_bActive) return;

	// Get / Display the framerate
	if (m_LastFrameRate != m_Timer.GetFrameRate())
	{
		m_LastFrameRate = m_Timer.GetFrameRate(FrameRate, 50);
		sprintf_s(TitleBuffer, _T("Game : %s"), FrameRate);
		SetWindowText(m_hWnd, TitleBuffer);

	} // End if Frame Rate Altered

	if (!m_bActive) return;

	if (!m_pPlayer->isAlive())
	{
		MessageBox(0, _T("U a loser!! U lost!"), _T("End of game"), MB_OK);
		m_pBBuffer->~BackBuffer();
		ShutDown();
		return;
	}

	
	if (m_pPlayer->isAlive()&&m_pPlayer->getScore()==100)
	{
		MessageBox(0, _T("Level 2!"), _T("Congrats!"), MB_OK);

		// Poll & Process input devices
	
		BuildObjects(8);
		SetupGameState(8);
		ProcessInput();

		// Animate the game objects
		AnimateObjects();

		// Drawing the game objects
		DrawObjects();
		
		return;
		
	}
	else
	if (m_pPlayer->isAlive() && m_pPlayer->getScore() ==350)
	{
		MessageBox(0, _T("Level 3!"), _T("Congrats!"), MB_OK);

		// Poll & Process input devices

		BuildObjects(9);
		SetupGameState(9);
		ProcessInput();

		// Animate the game objects
		AnimateObjects();

		// Drawing the game objects
		DrawObjects();

		return;
	}
	else
		if (m_pPlayer->isAlive() && m_pPlayer->getScore() >= 2000)
		{
			MessageBox(0, _T("Congratulations! You won! Game finished!"), _T("End of game"), MB_OK);
			m_pBBuffer->~BackBuffer();
			ShutDown();
			return;
		}
	// Poll & Process input devices
	ProcessInput();

	// Animate the game objects
	AnimateObjects();

	// Drawing the game objects
	DrawObjects();
}

//-----------------------------------------------------------------------------
// Name : ProcessInput () (Private)
// Desc : Simply polls the input devices and performs basic input operations
//-----------------------------------------------------------------------------
void CGameApp::ProcessInput()
{
	static UINT	 fTimerMoveEnemy;
	static UCHAR pKeyBuffer[256];
	//static UCHAR pKeyBuffer2[256];

	ULONG		Direction = 0;
	ULONG		Direction3 = 0;
	ULONG		Direction4 = 0;
	ULONG		Direction5 = 0;
	POINT		CursorPos;
	float		X = 0.0f, Y = 0.0f;

	// Retrieve keyboard state
	if (!GetKeyboardState(pKeyBuffer)) return;
	int counterUP = 0;
	int counterDOWN = 0;
	// Check the relevant keys

	int pos = m_pPlayer->Position().x;
	
	if (pKeyBuffer['1'] & 0xF0) {

		m_pPlayer->Position() = Vec2(pos, 900);
		CPlayer::SPEED_STOP;	
	}

	if (pKeyBuffer['2'] & 0xF0) {

		m_pPlayer->Position() = Vec2(pos, 450);
		CPlayer::SPEED_STOP;
	}


	if (pKeyBuffer['3'] & 0xF0) {

			m_pPlayer->Position() = Vec2(pos, 250);
			CPlayer::SPEED_STOP;

	}

	if (pKeyBuffer['4'] & 0xF0) {

		m_pPlayer->Position() = Vec2(pos, 150);
		CPlayer::SPEED_STOP;
	}

	if (pKeyBuffer[VK_LEFT] & 0xF0) Direction |= CPlayer::DIR_LEFT;
	if (pKeyBuffer[VK_RIGHT] & 0xF0) Direction |= CPlayer::DIR_RIGHT;

	Direction4 |= CEnemy::DIR_RIGHT;
	Direction3 |= CEnemy::DIR_LEFT;
	Direction5 |= CBonus::DIR_BACKWARD;
	m_pEnemy->MoveLeft(Direction3);
	m_pEnemy2->MoveLeft(Direction4);
	m_pEnemy3->MoveLeft(Direction4);
	m_pEnemy4->MoveLeft(Direction3);
	m_pEnemy5->MoveLeft(Direction4);
	m_pEnemy6->MoveLeft(Direction4);

	m_pBonus->MoveLeft(Direction3);
	m_pBonus2->MoveLeft(Direction4);
	m_pBonus3->MoveDown(Direction5);
	m_pBonus4->MoveDown(Direction5);
	// Move the player
	m_pPlayer->Move(Direction);

	

	// Now process the mouse (if the button is pressed)
	if ( GetCapture() == m_hWnd )
	{
		// Hide the mouse pointer
		SetCursor( NULL );

		// Retrieve the cursor position
		GetCursorPos( &CursorPos );

		// Reset our cursor position so we can keep going forever :)
		SetCursorPos( m_OldCursorPos.x, m_OldCursorPos.y );

	} // End if Captured
}


//-----------------------------------------------------------------------------
// Name : AnimateObjects () (Private)
// Desc : Animates the objects we currently have loaded.
//-----------------------------------------------------------------------------
void CGameApp::AnimateObjects()
{
	m_pPlayer->Update(m_Timer.GetTimeElapsed());
	
	m_pEnemy->Update(m_Timer.GetTimeElapsed());
	m_pEnemy2->Update(m_Timer.GetTimeElapsed());
	m_pEnemy3->Update(m_Timer.GetTimeElapsed());
	m_pEnemy4->Update(m_Timer.GetTimeElapsed());
	m_pEnemy5->Update(m_Timer.GetTimeElapsed());
	m_pEnemy6->Update(m_Timer.GetTimeElapsed());

	m_pBonus->Update(m_Timer.GetTimeElapsed());
	m_pBonus2->Update(m_Timer.GetTimeElapsed());
	m_pBonus3->Update(m_Timer.GetTimeElapsed());
	m_pBonus4->Update(m_Timer.GetTimeElapsed());

	ManageCollisions();
	
}

//-----------------------------------------------------------------------------
// Name : DrawObjects () (Private)
// Desc : Draws the game objects
//-----------------------------------------------------------------------------
void CGameApp::DrawObjects()
{

	m_pBBuffer->reset();

	m_imgBackground.Paint(m_pBBuffer->getDC(), 0, 0);

	m_pPlayer->Draw();

	m_pBonus->Draw();

	m_pBonus2->Draw();

	m_pBonus3->Draw();

	m_pBonus4->Draw();

	m_pEnemy->Draw();

	m_pEnemy2->Draw();

	m_pEnemy3->Draw();

	m_pEnemy4->Draw();

	m_pEnemy5->Draw();

	m_pEnemy6->Draw();

	
	static UCHAR pKeyBuffer2[256];
	if (!GetKeyboardState(pKeyBuffer2)) return;
	
	m_pBBuffer->present();

}

// player <-> bonus

bool CGameApp::CheckCollision(CPlayer &obj1, CBonus &obj2)
{
	bool OutsideBottom = obj1.Position().y + obj1.Height() / 2 <
		obj2.Position().y - obj2.Height() / 2;//Rect1.Bottom < Rect2.Top

	bool OutsideTop = obj1.Position().y - obj1.Height() / 2 >
		obj2.Position().y + obj2.Height() / 2;//Rect1.Top > Rect2.Bottom

	bool OutsideLeft = obj1.Position().x - obj1.Width() / 2 >
		obj2.Position().x + obj2.Width() / 2;//Rect1.Left > Rect2.Right

	bool OutsideRight = obj1.Position().x + obj1.Width() / 2 <
		obj2.Position().x - obj2.Width() / 2;//Rect1.Right < Rect2.Left

	bool ok = !(OutsideBottom || OutsideTop || OutsideLeft || OutsideRight);
	if (ok)
	{
		return true;
	}
	return false;
}

//player <-> obstacle

bool CGameApp::CheckCollision2(CPlayer &obj1, CEnemy &obj2)
{
	bool OutsideBottom = obj1.Position().y + obj1.Height() / 2 <
		obj2.Position().y - obj2.Height() / 2;//Rect1.Bottom < Rect2.Top

	bool OutsideTop = obj1.Position().y - obj1.Height() / 2 >
		obj2.Position().y + obj2.Height() / 2;//Rect1.Top > Rect2.Bottom

	bool OutsideLeft = obj1.Position().x - obj1.Width() / 2 >
		obj2.Position().x + obj2.Width() / 2;//Rect1.Left > Rect2.Right

	bool OutsideRight = obj1.Position().x + obj1.Width() / 2 <
		obj2.Position().x - obj2.Width() / 2;//Rect1.Right < Rect2.Left

	bool ok = !(OutsideBottom || OutsideTop || OutsideLeft || OutsideRight);
	if (ok)
	{
		return true;
	}
	return false;
}

void CGameApp::ManageCollisions()
{
	static UINT	fTimer;

	// player <-> bonus

	if (CheckCollision(*m_pPlayer, *m_pBonus))
	{
		m_pPlayer->addLife(m_pBBuffer);
		m_pBonus->Position() = Vec2(-3000, 250);
		fTimer = SetTimer(m_hWnd, 1, 200, NULL);
	}

	if (CheckCollision(*m_pPlayer, *m_pBonus2))
	{
		m_pPlayer->addLife(m_pBBuffer);
		m_pBonus2->Position() = Vec2(3000, 250);
		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
	}

	if (CheckCollision(*m_pPlayer, *m_pBonus3))
	{
		m_pPlayer->incrementScore(100);
		m_pBonus3->Position() = Vec2(1000, 950);
		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
	}

	if (CheckCollision(*m_pPlayer, *m_pBonus4))
	{
		m_pPlayer->incrementScore(100);
		m_pBonus4->Position() = Vec2(700, 1000);
		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy))
	{
	
		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy->Position() = Vec2(1600, 450);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy2))
	{
		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy2->Position() = Vec2(-1600, 200);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy3))
	{

		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy3->Position() = Vec2(2000, 550);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy4))
	{

		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy4->Position() = Vec2(1500, 450);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy5))
	{

		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy5->Position() = Vec2(3000, 700);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}

	if (CheckCollision2(*m_pPlayer, *m_pEnemy6))
	{

		fTimer = SetTimer(m_hWnd, 1, 100, NULL);
		m_pEnemy6->Position() = Vec2(-1000, 450);
		m_pPlayer->deleteLife();
		//m_pPlayer->decreaselife();
	}


}


