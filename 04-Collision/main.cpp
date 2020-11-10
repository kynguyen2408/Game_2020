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
#include "Panther.h"
#include "Roi.h"
#include "Dao.h"
#include "Riu.h"
#include "Fire.h"
#include "Map.h"
#include "Nen.h"
#include "Stairs.h"
#include "Camera.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 550
#define SCREEN_HEIGHT 475

#define MAX_FRAME_RATE 120

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20
#define ID_TEX_ROI 30
#define ID_TEX_DAO 40
#define ID_TEX_NEN 50


CGame *game;

CMario *mario;
CGoomba *goomba;
CPanther* panther;
CRoi* roi;
CFire* fire;
Map* map;
Nen* nen;
CStairs* stairs;
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
		if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_DOWN)==NULL)
		{
			DebugOut(L"[INFO] A danh: %d\n", KeyCode);
			if (mario->hitting == false)
			{
				if (mario->allowCreateSecondWeapon == true) mario->SetState(MARIO_STATE_THROW);
				else mario->SetState(MARIO_STATE_HIT);
			}
		}
		else
		{
			DebugOut(L"[INFO] A: %d\n", KeyCode);
			if (mario->hitting == false)
				mario->SetState(MARIO_STATE_HIT);
		}
		break;
	case DIK_1:
		DebugOut(L"[INFO] 1: %d\n", KeyCode);
		mario->secondWeapon = DAO_TYPE;
		break;
	case DIK_2:
		DebugOut(L"[INFO] 2: %d\n", KeyCode);
		mario->secondWeapon = RIU_TYPE;
		break;
	case DIK_3:
		DebugOut(L"[INFO] 3: %d\n", KeyCode);
		mario->secondWeapon = FIRE_TYPE;
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
			else if (game->IsKeyDown(DIK_UP))
			{
				mario->SetState(MARIO_STATE_IDLE);
				mario->allowCreateSecondWeapon == false;

			}
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

	textures->Add(ID_TEX_MARIO, L"textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ENEMY, L"textures\\Enemies.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROI, L"textures\\morningstar.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DAO, L"textures\\Sub_weapons.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_NEN, L"textures\\Static_Obj.png", D3DCOLOR_XRGB(176, 224, 24));

	textures->Add(99, L"textures\\title1.png", D3DCOLOR_XRGB(255, 255, 255));

	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	
	LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);

	sprites->Add(10001, 434, 200, 469, 260, texMario);

	sprites->Add(10002, 375, 200, 401, 260, texMario);
	sprites->Add(10003, 313, 200, 346, 260, texMario);
	sprites->Add(10004, 260, 200, 284, 260, texMario);

	sprites->Add(10005, 197, 200, 226, 260, texMario); // ngồi quay mặt qua phải

	sprites->Add(10011, 13, 2, 46, 63, texMario);

	sprites->Add(10012, 78, 2, 104, 63, texMario);
	sprites->Add(10013, 135, 2, 168, 63, texMario);
	sprites->Add(10014, 195, 2, 220, 63, texMario);

	sprites->Add(10015, 252, 2, 284, 63, texMario); // ngồi quay mặt qua trái

	// danh phai dung
	sprites->Add(10006, 120, 202, 168, 261, texMario);
	sprites->Add(10007, 75, 202, 108, 261, texMario);
	sprites->Add(10008, 16, 204, 58, 261, texMario);
	// danh trai dung
	sprites->Add(10016, 312, 4, 359, 63, texMario);
	sprites->Add(10017, 373, 4, 405, 63, texMario);
	sprites->Add(10018, 421, 6, 464, 63, texMario);
	// danh phai ngoi`
	sprites->Add(100061, 0, 265, 48, 327, texMario);
	sprites->Add(100062, 437, 331, 468, 393, texMario);
	sprites->Add(100063, 374, 333, 418, 393, texMario);
	// danh trai ngoi`
	sprites->Add(100161, 432, 67, 480, 129, texMario);
	sprites->Add(100162, 11, 133, 43, 195, texMario);
	sprites->Add(100163, 60, 135, 105, 195, texMario);

	// phi dao phai dung
	sprites->Add(10006, 120, 202, 168, 261, texMario);

	// phi dao trai dung
	sprites->Add(10016, 312, 4, 359, 63, texMario);


	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ENEMY);
	//ghost
	sprites->Add(20001, 255, 0, 288, 64, texEnemy);
	sprites->Add(20002, 292, 0, 324, 64, texEnemy);

	sprites->Add(20011, 362, 0, 392, 64, texEnemy);
	sprites->Add(20012, 324, 0, 356, 64, texEnemy);

	//Panther
	sprites->Add(20003, 464, 66, 510, 95, texEnemy);

	sprites->Add(20004, 392, 64, 447, 95, texEnemy);
	sprites->Add(20005, 324, 64, 383, 95, texEnemy);
	sprites->Add(20006, 256, 66, 319, 91, texEnemy);

	sprites->Add(20013, 0, 66, 47, 95, texEnemy);

	sprites->Add(20014, 64, 64, 119, 95, texEnemy);
	sprites->Add(20015, 128, 64, 187, 95, texEnemy);
	sprites->Add(20016, 192, 66, 255, 91, texEnemy);

	LPDIRECT3DTEXTURE9 texRoi = textures->Get(ID_TEX_ROI);

	sprites->Add(40004, 0, 204, 159, 272, texRoi);
	sprites->Add(40003, 159, 204, 318, 272, texRoi);
	sprites->Add(40002, 318, 204, 477, 272, texRoi);
	sprites->Add(40001, 476, 204, 635, 272, texRoi);

	sprites->Add(40011, 0, 0, 159, 68, texRoi);
	sprites->Add(40012, 159, 0, 318, 68, texRoi);
	sprites->Add(40013, 318, 0, 477, 68, texRoi);

	
	LPDIRECT3DTEXTURE9 texNen = textures->Get(ID_TEX_NEN);
	sprites->Add(60001, 400, 0, 432, 42, texNen); // nen

	LPDIRECT3DTEXTURE9 texDao = textures->Get(ID_TEX_DAO);
	sprites->Add(50001, 32, 64, 63, 81, texDao);  //dao phi qua phai

	sprites->Add(50011, 65, 64, 95, 81, texDao); //dao phi qua trai

	//riu phai
	sprites->Add(50002, 62, 0, 91, 27, texDao); 
	sprites->Add(50003, 92, 0, 121, 27, texDao);
	//riu trai
	sprites->Add(50012, 32, 0, 61, 27, texDao);
	sprites->Add(50013, 122, 0, 151, 27, texDao);

	//lua
	sprites->Add(50004, 8, 100, 23, 115, texDao); //binh`
	sprites->Add(50005, 42, 98, 53, 121, texDao); //lua nho
	sprites->Add(50006, 64, 96, 95, 121, texDao); //lua lon


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

	ani = new CAnimation(100); //dah phai dung
	ani->Add(10001, 10);
	ani->Add(10006);
	ani->Add(10007);
	ani->Add(10008);
	animations->Add(1000, ani);

	ani = new CAnimation(100); //dah trai dung
	ani->Add(10011, 10);
	ani->Add(10016);
	ani->Add(10017);
	ani->Add(10018);
	animations->Add(1001, ani);

	ani = new CAnimation(100); //dah phai ngoi
	ani->Add(10005, 10);
	ani->Add(100061);
	ani->Add(100062);
	ani->Add(100063);
	animations->Add(1100, ani);

	ani = new CAnimation(100); //dah trai ngoi
	ani->Add(10015, 10);
	ani->Add(100161);
	ani->Add(100162);
	ani->Add(100163);
	animations->Add(1101, ani);

	//ani = new CAnimation(100);		// Mario die
	//ani->Add(10099);
	//animations->Add(599, ani);

	

	ani = new CAnimation(100);		// Goomba walk
	ani->Add(20001);
	ani->Add(20002);
	animations->Add(801, ani);

	ani = new CAnimation(100);
	ani->Add(20011);
	ani->Add(20012);
	animations->Add(800, ani);

	ani = new CAnimation(100);		// Panther right
	ani->Add(20003);
	animations->Add(910, ani);

	ani = new CAnimation(100);		// Panther left
	ani->Add(20013);
	animations->Add(911, ani);

	ani = new CAnimation(100);		// Panther run right
	ani->Add(20004);
	ani->Add(20005);
	ani->Add(20006);
	animations->Add(900, ani);

	ani = new CAnimation(100);		// Panther run left
	ani->Add(20014);
	ani->Add(20015);
	ani->Add(20016);
	animations->Add(901, ani);

	ani = new CAnimation(100);   //roi phai
	ani->Add(40004, 10);	// frame trống
	ani->Add(40001);
	ani->Add(40002);
	ani->Add(40003);
	animations->Add(300, ani);

	ani = new CAnimation(100);   //roi trai
	ani->Add(40004, 10);
	ani->Add(40011);
	ani->Add(40012);
	ani->Add(40013);
	animations->Add(301, ani);

	ani = new CAnimation(100);   //dao phai
	ani->Add(50001);
	animations->Add(3000, ani);

	ani = new CAnimation(100);   //dao trai
	ani->Add(50011);
	animations->Add(3001, ani);

	ani = new CAnimation(100);   //riu phai
	ani->Add(50002);
	ani->Add(50003);
	animations->Add(4000, ani);

	ani = new CAnimation(100);   //riu phai
	ani->Add(50012);
	ani->Add(50013);
	animations->Add(4001, ani);

	ani = new CAnimation(100); //lua binh`
	ani->Add(50004);
	animations->Add(5000, ani);

	ani = new CAnimation(300);   //lua chay
	ani->Add(50005);
	ani->Add(50006);
	animations->Add(5001, ani);

	ani = new CAnimation(100);		// nen
	ani->Add(60001);
	animations->Add(902, ani);


	mario = mario->GetInstance();
	mario->AddAnimation(400);		// idle right
	mario->AddAnimation(401);		// idle left
	mario->AddAnimation(500);		// walk right
	mario->AddAnimation(501);		// walk left
	mario->AddAnimation(600);      // jump right
	mario->AddAnimation(601);		// jump left
	mario->AddAnimation(700);      // sit right
	mario->AddAnimation(701);		// sit left
	mario->AddAnimation(1000);      //hit right
	mario->AddAnimation(1001);      //hit left
	mario->AddAnimation(1100);      //hit right
	mario->AddAnimation(1101);      //hit left
	//mario->AddAnimation(599);		// die
	
	mario->SetPosition(mario->mario_x, mario->mario_y);
	objects.push_back(mario);

	//gach duoi dat
	CBrick* brick = new CBrick();
	brick->SetPosition(0, 368);
	brick->SetWH(2000, BRICK_HEIGHT);
	objects.push_back(brick);

	//gach tren
	CBrick* brick1 = new CBrick();
	brick1->SetPosition(400, 280);
	brick1->SetWH(280, BRICK_HEIGHT);
	objects.push_back(brick1);

	// Goombas 
	/*for (int i = 0; i < 4; i++)
	{
		goomba = new CGoomba();
		goomba->AddAnimation(800);
		goomba->AddAnimation(801);
		goomba->AddAnimation(802);
		goomba->SetPosition(400 + i*60, 306);
		goomba->SetState(GOOMBA_STATE_WALKING_LEFT);
		objects.push_back(goomba);
	}*/
	// Panther
	panther = new CPanther();
	panther->SetPosition(500, 250);
	objects.push_back(panther);

	//nen
	Nen *nen = new Nen();
	nen->AddAnimation(902);
	nen->AddAnimation(400);
	nen->SetPosition(400 + 0 * 16.0f, 320);
	objects.push_back(nen);
	// Cau thang
	CStairs* stairs = new CStairs();
	stairs->SetPosition(300, 300);
	objects.push_back(stairs);

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

	if (mario->hitting == true )
	{
		float x, y;

		mario->GetPosition(x, y);
		if (mario->throwing == true && mario->allowCreateSecondWeapon==true && mario->secondWeapon!=0) //tao vu khi phu.
		{
			if(mario->secondWeapon	== DAO_TYPE)
			{
				objects.push_back(new CDao());
			}
			else if (mario->secondWeapon == RIU_TYPE)
			{
				objects.push_back(new CRiu());
			}
			else if (mario->secondWeapon == FIRE_TYPE) {
				fire = new CFire();
				objects.push_back(fire);
			}
			mario->throwing = false;
			mario->allowCreateSecondWeapon = false;
		}
		else if (mario->allowCreateWhip == true) //tao roi
		{
			roi = new CRoi();
			if (mario->nx > 0)
			{
				roi->AddAnimation(300);
			}
			else
			{
				roi->AddAnimation(301);
			}
			roi->animations[0]->isLastFrame = false;
			roi->animations[0]->currentFrame = -1;

			objects.push_back(roi);
		}
		mario->allowCreateWhip = false;

	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt,&coObjects);
	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->dead == true) {
			// cho phep tao ra vu khi phu. 1 lan`
			if (objects[i]->type == RIU_TYPE || objects[i]->type == DAO_TYPE || objects[i]->type == FIRE_TYPE)
				mario->allowCreateSecondWeapon = true;
			objects.erase(objects.begin() + i);
		}
	}
	mario->vaChamTuong(dt, &wallObjects);
	if(fire != NULL)
		fire->VaChamDat(dt, &wallObjects);
	if(panther!=NULL)
		panther->VaChamDat(dt, &wallObjects);



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