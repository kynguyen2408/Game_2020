/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Roi.h"
#include "Dao.h"
#include "Riu.h"
#include "Fire.h"
#include "Map.h"
#include "Nen.h"
#include "Camera.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 550
#define SCREEN_HEIGHT 475

#define MAX_FRAME_RATE 120

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 80	
#define ID_TEX_MISC 20
#define ID_TEX_ROI 30
#define ID_TEX_DAO 40
#define ID_TEX_NEN 50
#define ID_TEX_ITEMS 60


CGame *game;

CMario *mario;
CGoomba *goomba;
CRoi* roi;
CDao* dao;
CRiu* riu;
CFire* fire;
Map* map;
Nen* nen;
CCamera* camera;
vector<LPGAMEOBJECT> objects;
CMario * CMario::__instance = NULL;
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
		DebugOut(L"[INFO] Space: %d\n", KeyCode);
		if (mario->sitting == false)
			mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
		{
			DebugOut(L"[INFO] A danh: %d\n", KeyCode);
			if (mario->hitting == false)
				mario->SetState(MARIO_STATE_LAUNCH);
		}
		else
		{
			DebugOut(L"[INFO] A: %d\n", KeyCode);
			if (mario->hitting == false)
				mario->SetState(MARIO_STATE_HIT);
		}
		break;
	case DIK_S:
		DebugOut(L"[INFO] S: %d\n", KeyCode);
		if (mario->hitting == false)
			mario->SetState(MARIO_STATE_THROW);
		break;
	case DIK_D:
		DebugOut(L"[INFO] D: %d\n", KeyCode);
		if (mario->hitting == false)
			mario->SetState(MARIO_STATE_BURN);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (mario->jumping == false && mario->hitting==false && mario->injured==false)
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
			if (game->IsKeyDown(DIK_RIGHT)) mario->SetState(MARIO_STATE_SIT_RIGHT);
			else if (game->IsKeyDown(DIK_LEFT)) mario->SetState(MARIO_STATE_SIT_LEFT);
			else if (game->IsKeyDown(DIK_UP)) mario->SetState(MARIO_STATE_IDLE);
			else mario->SetState(MARIO_STATE_SIT);
		}
		else
		{
			if(mario->GetState() != MARIO_STATE_INJURED || mario->vy > 0)
				mario->SetState(MARIO_STATE_IDLE);
		}
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

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{

	CTextures * textures = CTextures::GetInstance();
	map = Map::GetInstance();
	map->LoadResources();


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	textures->LoadResources();
	sprites->LoadResources();
	animations->LoadResources();

	mario = mario->GetInstance();
	mario->AddAnimation(400);		// idle right
	mario->AddAnimation(401);		// idle left

	mario->AddAnimation(500);		// walk right
	mario->AddAnimation(501);		// walk left

	mario->AddAnimation(509);      // jump right
	mario->AddAnimation(508);		// jump left

	mario->AddAnimation(510);      // sit right
	mario->AddAnimation(511);		// sit left

	mario->AddAnimation(513);      //hit right
	mario->AddAnimation(512);      //hit left

	mario->AddAnimation(514);      //hit sit right
	mario->AddAnimation(515);      //hit sit left
	//mario->AddAnimation(599);		// die
	
	mario->SetPosition(mario->mario_x, mario->mario_y);
	objects.push_back(mario);

	
	


	for (int i = 0; i < 1; i++)
	{
		CBrick *brick = new CBrick();
		brick->SetPosition(0 + i*16.0f, 384);
		objects.push_back(brick);
	}

	//// and Goombas 
	//for (int i = 0; i < 4; i++)
	//{
	//	goomba = new CGoomba();
	//	goomba->AddAnimation(902); // trai
	//	goomba->AddAnimation(901); // phai 
	//	goomba->SetPosition(200 + i*60, 340);
	//	goomba->SetState(GOOMBA_STATE_WALKING_LEFT);
	//	objects.push_back(goomba);
	//}
	//nen
	Nen *nen = new Nen();
	nen->setTypeItems(NEN_ANI_ITEM_WHIP);
	nen->SetPosition(400 + 0 * 16.0f, 320);
	objects.push_back(nen);

	Nen *nen2 = new Nen();
	nen2->setTypeItems(NEN_ANI_ITEM_AXE);
	nen2->SetPosition(700 + 0 * 16.0f, 320);
	objects.push_back(nen2);

	
	//tao camera
	camera = camera->GetInstance();
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> wallObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		if (objects[i]->type == BRICK_TYPE) {
			wallObjects.push_back(objects[i]);
		}
		else coObjects.push_back(objects[i]);
	}
	if (mario->hitting == true && mario->allowCreateWhip==true )
	{
		float x, y;

		mario->GetPosition(x, y);
		if (mario->launching == true)
		{ 
				dao = new CDao();
				objects.push_back(dao);
				mario->launching = false;
		}
		else if (mario->throwing == true)
		{
			riu = new CRiu();
			objects.push_back(riu);
			mario->throwing = false;
		}
		else if (mario->burning == true)
		{
			fire = new CFire();
			objects.push_back(fire);
			mario->burning = false;
		}
		else
		{
			roi = new CRoi();
			if (mario->nx > 0)
			{
				roi->AddAnimation(603); // roi phai
				roi->AddAnimation(604); // roi phai 2
				roi->AddAnimation(605); // roi phai 3
			}
			else
			{
				roi->AddAnimation(600); // roi trai
				roi->AddAnimation(601); // roi trai 2
				roi->AddAnimation(602); // roi trai 3
			}
			roi->animations[0]->isLastFrame = false;
			roi->animations[0]->currentFrame = -1;
			objects.push_back(roi);
		}
		mario->allowCreateWhip = false;
	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->type == NEN_TYPE) {
			objects[i]->Update(dt, &wallObjects);
		}
		else {
			objects[i]->Update(dt, &coObjects);

		}
	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->dead == true)
			objects.erase(objects.begin() + i);
	}
	

	
	mario->vaChamTuong(dt, &wallObjects);
	if(fire != NULL)
		fire->VaChamDat(dt, &wallObjects);



	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	camera->x = cx;
	camera->Update();
	CGame::GetInstance()->SetCamPos(camera->x,0.0f);
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
		map->Render();
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}