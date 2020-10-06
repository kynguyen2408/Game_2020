/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 03 - KEYBOARD AND OBJECT STATE

	This sample illustrates how to:

		1/ Process keyboard input
		2/ Control object state with keyboard events
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Ghost.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 500

#define MAX_FRAME_RATE 90

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

CGame *game;
CMario *mario;
CGhost * ghost, * ghost1, * ghost2;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->sitting == false)
			mario->SetState(MARIO_STATE_JUMP);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	if (mario->jumping == false)
	{
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (game->IsKeyDown(DIK_DOWN))
				mario->SetState(MARIO_STATE_SIT_RIGHT);
			else if (game->IsKeyDown(DIK_LEFT)) mario->SetState(MARIO_STATE_IDLE);
			else mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{

			if (game->IsKeyDown(DIK_DOWN))
				mario->SetState(MARIO_STATE_SIT_LEFT);
			else if (game->IsKeyDown(DIK_RIGHT)) mario->SetState(MARIO_STATE_IDLE);
			else mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else if (game->IsKeyDown(DIK_DOWN))
		{
			if(game->IsKeyDown(DIK_RIGHT)) mario->SetState(MARIO_STATE_SIT_RIGHT);
			else if (game->IsKeyDown(DIK_LEFT)) mario->SetState(MARIO_STATE_SIT_LEFT);
			else mario->SetState(MARIO_STATE_SIT);
		}
		else mario->SetState(MARIO_STATE_IDLE);
	}
	
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_MARIO, L"textures\\simon.png",D3DCOLOR_XRGB(176, 224, 248));
	textures->Add(ID_TEX_ENEMY, L"textures\\Enemies.png", D3DCOLOR_XRGB(176, 224, 248));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	sprites->Add(10001, 434, 200, 469, 260, texMario);

	sprites->Add(10002, 375, 200, 401, 260, texMario);
	sprites->Add(10003, 313, 200, 346, 260, texMario);
	sprites->Add(10004, 260, 200, 284, 260, texMario);

	sprites->Add(10005, 197, 200, 226, 245, texMario);

	sprites->Add(10011, 13, 2, 46, 64, texMario);

	sprites->Add(10012, 78, 2, 104, 64, texMario);
	sprites->Add(10013, 135, 2, 168, 64, texMario);
	sprites->Add(10014, 195, 2, 220, 64, texMario);

	sprites->Add(10015, 252, 2, 284, 46, texMario);
	

	LPDIRECT3DTEXTURE9 texGhost = textures->Get(ID_TEX_ENEMY);

	sprites->Add(20001, 255, 0, 288, 64, texGhost);
	sprites->Add(20002, 292, 0, 324, 64, texGhost);

	sprites->Add(20011, 362, 0, 392, 64, texGhost);
	sprites->Add(20012, 324, 0, 356, 64, texGhost);

	LPANIMATION ani;

	ani = new CAnimation(100);	
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	animations->Add(401, ani);


	ani = new CAnimation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(10004);
	animations->Add(500, ani);

	ani = new CAnimation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(10014);
	animations->Add(501, ani);

	ani = new CAnimation(100);
	ani->Add(10005);
	animations->Add(600, ani);

	ani = new CAnimation(100);
	ani->Add(10015);
	animations->Add(601, ani);

	ani = new CAnimation(100);
	ani->Add(10005);
	animations->Add(700, ani);

	ani = new CAnimation(100);
	ani->Add(10015);
	animations->Add(701, ani);


	ani = new CAnimation(100);
	ani->Add(20001);
	ani->Add(20002);
	animations->Add(800, ani);

	ani = new CAnimation(100);
	ani->Add(20011);
	ani->Add(20012);
	animations->Add(801, ani);


	mario = new CMario();
	CMario::AddAnimation(400);		// idle right
	CMario::AddAnimation(401);		// idle left
	CMario::AddAnimation(500);		// walk right
	CMario::AddAnimation(501);		// walk left
	CMario::AddAnimation(600);      // jump right
	CMario::AddAnimation(601);		// jump left
	CMario::AddAnimation(700);      // sit right
	CMario::AddAnimation(701);		// sit left

	ghost = new CGhost();
	CGhost::AddAnimation(800);
	CGhost::AddAnimation(801);

	ghost1 = new CGhost();
	ghost2 = new CGhost();


	mario->SetPosition(0.0f, 100.0f);
	
	ghost->SetPosition(0.0f, 150.0f);
	ghost1->SetPosition(10.0f, 200.0f);
	ghost2->SetPosition(20.0f, 250.0f);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	ghost->Update(dt);
	ghost1->Update(dt);
	ghost2->Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		mario->Render();
		ghost->Render();
		ghost1->Render();
		ghost2->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();
	Run();

	return 0;
}