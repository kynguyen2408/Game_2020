
#include "PlayScene.h"

#include "debug.h"
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
#include "Aquaman.h"
#include "Ball.h"
#include <string>
#include <fstream>
#include "Bat.h"
using namespace std;

#define ID_TEX_MARIO 0
#define ID_TEX_ENEMY 80	
#define ID_TEX_MISC 20
#define ID_TEX_ROI 30
#define ID_TEX_DAO 40
#define ID_TEX_NEN 50
#define ID_TEX_ITEMS 60
#define ID_TEX_AQUAMAN 70
#define ID_TEX_BALL 80


#define SCREEN_WIDTH 550
#define SCREEN_HEIGHT 475


CMario* mario;
CGoomba* goomba;
CPanther* panther;
CRoi* roi;
CFire* fire;
Map* map;
Nen* nen;
CBall* ball;
CStairs* stairs;
CCamera* camera;
vector<LPGAMEOBJECT> objects;
CMario* CMario::__instance = NULL;
CAquaman* CAquaman::__instance = NULL;

CGame* maingame;
class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander* keyHandler;


void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		DebugOut(L"[INFO] Space: %d\n", KeyCode);
		if (mario->sitting == false && mario->uppingRight == false && mario->uppingLeft == false)
			mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		if (maingame->IsKeyDown(DIK_UP) && maingame->IsKeyDown(DIK_DOWN) == NULL)
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
	case DIK_6:
		mario->levelMap = 2;
		map->LoadResources(mario->levelMap);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	// disable control key when Mario die 
	if (mario->jumping == false && mario->hitting == false && mario->injured == false)
	{
		if (maingame->IsKeyDown(DIK_RIGHT))
		{
			if (maingame->IsKeyDown(DIK_DOWN) && !mario->uppingLeft && !mario->uppingRight)
				mario->SetState(MARIO_STATE_SIT_RIGHT);
			else if (maingame->IsKeyDown(DIK_LEFT))
			{
				if (mario->uppingRight) mario->SetState(MARIO_STATE_STAND_STAIRS);
				mario->SetState(MARIO_STATE_IDLE);
			}
			else if (mario->uppingRight) mario->SetState(MARIO_STATE_UPSTAIRS_RIGHT);
			else if (mario->uppingLeft) mario->SetState(MARIO_STATE_DOWNSTAIRS_LEFT);
			else mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (maingame->IsKeyDown(DIK_LEFT))
		{
			if (maingame->IsKeyDown(DIK_DOWN) && !mario->uppingLeft && !mario->uppingRight)
				mario->SetState(MARIO_STATE_SIT_LEFT);
			/*else if (game->IsKeyDown(DIK_RIGHT))
			{
				if(mario->upping) mario->SetState(MARIO_STATE_STAND_STAIRS);
				else mario->SetState(MARIO_STATE_IDLE);
			}*/
			else if (mario->uppingRight) mario->SetState(MARIO_STATE_DOWNSTAIRS_RIGHT);
			else if (mario->uppingLeft) mario->SetState(MARIO_STATE_UPSTAIRS_LEFT);
			else mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else if (maingame->IsKeyDown(DIK_DOWN))
		{
			if (mario->allowDownStairsRight == true)
			{
				mario->SetState(MARIO_STATE_DOWNSTAIRS_RIGHT);
			}
			else if (mario->allowDownStairsLeft == true)
				mario->SetState(MARIO_STATE_DOWNSTAIRS_LEFT);
			else if (maingame->IsKeyDown(DIK_RIGHT))
				mario->SetState(MARIO_STATE_SIT_RIGHT);
			else if (maingame->IsKeyDown(DIK_LEFT))
				mario->SetState(MARIO_STATE_SIT_LEFT);
			else if (maingame->IsKeyDown(DIK_UP))
			{
				if (mario->uppingRight) mario->SetState(MARIO_STATE_STAND_STAIRS);
				else mario->SetState(MARIO_STATE_IDLE);
				mario->allowCreateSecondWeapon = false;
			}
			else if (mario->uppingRight) mario->SetState(MARIO_STATE_DOWNSTAIRS_RIGHT);
			else if (mario->uppingLeft) mario->SetState(MARIO_STATE_DOWNSTAIRS_LEFT);
			else if(mario->allowSit==true)
			{
				 mario->SetState(MARIO_STATE_SIT);
			}
		}
		else if (maingame->IsKeyDown(DIK_UP))
		{
			if (mario->allowUpStairsRight == true)
			{
				
				if(mario->x - mario->StairX >= 0.1 && !mario->uppingRight)
					mario->SetState(MARIO_STATE_WALKING_LEFT);
				if(mario->x + MARIO_BIG_BBOX_WIDTH - mario->StairX >= 0.1 && mario->x <= mario->StairX)
					mario->SetState(MARIO_STATE_UPSTAIRS_RIGHT);
				if (mario->x + MARIO_BIG_BBOX_WIDTH - mario->StairX <= 0.1)
					mario->SetState(MARIO_STATE_WALKING_RIGHT);
			}
				
			else if (mario->allowUpStairsLeft == true)
				mario->SetState(MARIO_STATE_UPSTAIRS_LEFT);
			else if (mario->uppingRight) mario->SetState(MARIO_STATE_UPSTAIRS_RIGHT);
			else if (mario->uppingLeft) mario->SetState(MARIO_STATE_UPSTAIRS_LEFT);
			else mario->SetState(MARIO_STATE_IDLE);
		}
		else
		{
			if (mario->GetState() != MARIO_STATE_INJURED || mario->vy > 0)
				mario->SetState(MARIO_STATE_IDLE);
			if (mario->uppingRight || mario->uppingLeft)
			{
				mario->SetState(MARIO_STATE_STAND_STAIRS);
			}

		}
	}
}
PlayScene::PlayScene()
{

}

void PlayScene::CreateFileGird(int level)
{
	ifstream File;

	char gridFileName[30];

	sprintf_s(gridFileName, "text\\Grid%d.txt", level);
	File.open(gridFileName);

	float posX, posY, width, height, id, posTarX, PosMoveSimonX, PosMoveSimonY;

	string str, type;
	while (!File.eof())
	{
		File >> str;		// read type of obj
		if (str == "FIRE_PILLAR")
		{
			File >> posX >> posY >> width >> height >> str;
			Nen* nen = new Nen();
			if (str == "MORNING_STAR") {
				nen->setTypeItems(NEN_ANI_ITEM_WHIP);
			}
			else if (str == "BIG_HEART") {
				nen->setTypeItems(NEN_ANI_ITEM_BIG_HEART);
			}
			else if (str == "KNIFE") {
				nen->setTypeItems(NEN_ANI_ITEM_KNIFE);
			}
			else if (str == "DOUBLE_SHOT") {
				nen->setTypeItems(NEN_ANI_ITEM_DOUBLESHOT);
			}
			else if (str == "SMALL_HEART") {
				nen->setTypeItems(NEN_ANI_ITEM_SMALL_HEART);
			}
			nen->SetPosition(posX, posY);
			nen->SetWH(width, height);
			objects.push_back(nen);

		}
		else if (str == "CANDLE")
		{
			File >> posX >> posY >> width >> height >> str;
			Nen* nen = new Nen();

			if (level == 2) {
				nen->SetState(CANDLE_STATE_SMALL);
				if (str == "SMALL_HEART") {
					nen->setTypeItems(NEN_ANI_ITEM_SMALL_HEART);
				}
			}

			nen->SetPosition(posX, posY);
			nen->SetWH(width, height);
			objects.push_back(nen);

		}
		else if (str == "BRICK") {

			File >> posX >> posY >> width >> height;
			CBrick * brick = new CBrick();
			brick->SetPosition(posX, posY);
			brick->SetWH(width, height);
			objects.push_back(brick);

		}
		/*else if (str == "GHOST") {
			File >> posX >> posY >> width >> height;
			posY += 16;
			CGoomba * ghost = new CGoomba();
			ghost->SetPosition(posX, posY);
			ghost->SetState(GOOMBA_STATE_WALKING_LEFT);
			ghost->SetWH(width, height);
			objects.push_back(ghost);

		}*/
		else if (str == "BLACKPANTHER") {
			File >> posX >> posY >> width >> height;
			CPanther * panther = new CPanther();
			panther->SetPosition(posX, posY);
			panther->SetState(PANTHER_STATE_IDLE);
			panther->SetWH(width, height);
			objects.push_back(panther);

		}
		else if (str == "BAT") {
			File >> posX >> posY >> width >> height;
			CBat * bat = new CBat();
			bat->SetPosition(posX, posY);
			bat->SetWH(width, height);
			objects.push_back(bat);

		}
		else if (str == "AQUAMAN") {
			File >> posX >> posY >> width >> height;
			CAquaman * aquaman = new CAquaman();
			aquaman->SetPosition(posX, posY);
			aquaman->SetState(AQUAMAN_STATE_SIT_RIGHT);
			aquaman->SetWH(width, height);
			objects.push_back(aquaman);

		}
		else if (str == "STAIR") {
			File >> posX >> posY >> width >> height >> str;
			height -= 10;
			if (str == "STAIR_BOTTOM_RIGHT") {
				CStairs* stairs = new CStairs(STAIRS_RIGHT_UP);
				stairs->SetPosition(posX, posY);
				stairs->SetWH(width, height);
				objects.push_back(stairs);

			}
			else if (str == "STAIR_TOP_LEFT") {
				CStairs* stairs = new CStairs(STAIRS_RIGHT_DOWN);
				stairs->SetPosition(posX, posY);
				stairs->SetWH(width, height);
				objects.push_back(stairs);

			}
			else if (str == "STAIR_BOTTOM_LEFT") {
				CStairs* stairs = new CStairs(STAIRS_LEFT_UP);
				stairs->SetPosition(posX, posY);
				stairs->SetWH(width, height);
				objects.push_back(stairs);

			}
			else if (str == "STAIR_TOP_RIGHT") {
				CStairs* stairs = new CStairs(STAIRS_LEFT_DOWN);
				stairs->SetPosition(posX, posY);
				stairs->SetWH(width, height);
				objects.push_back(stairs);

			}
		}

		//case STAIR:
		//{
		//	GridFile >> posX >> posY >> width >> height >> type;
		//	GridFile >> left >> top >> right >> bot;
		//	auto stair = new CStair(TYPEString[type]);
		//	stair->SetPosition(posX, posY);
		//	stair->width = width;
		//	stair->height = height;

		//	loop(r, top, bot)
		//		loop(c, left, right)
		//		cells[r][c]->objects.insert(stair);
		//	break;
		//}
	}

	File.close();

}

void PlayScene::LoadResources()
{
	maingame = maingame->GetInstance();
	keyHandler = new CSampleKeyHander();
	maingame->InitKeyboard(keyHandler);

	CTextures* textures = CTextures::GetInstance();
	


	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	textures->LoadResources();
	sprites->LoadResources();
	animations->LoadResources();

	mario = mario->GetInstance();
	mario->SetPosition(1000, mario->mario_y);
	

	map = Map::GetInstance();
	map->LoadResources(mario->levelMap);

	
	CreateFileGird(1);

	// Cau thang
	//CStairs* stairs = new CStairs(STAIRS_RIGHT_UP);
	//stairs->SetPosition(300, 366);
	//objects.push_back(stairs);

	//CStairs* stairs1 = new CStairs(STAIRS_RIGHT_DOWN);
	//stairs1->SetPosition(375, 200);
	//objects.push_back(stairs1);

	//CStairs* stairs2 = new CStairs(STAIRS_LEFT_UP);
	//stairs2->SetPosition(730, 366);
	//objects.push_back(stairs2);

	//CStairs* stairs3 = new CStairs(STAIRS_LEFT_DOWN);
	//stairs3->SetPosition(650, 200);
	//objects.push_back(stairs3);

	//tao camera
	camera = camera->GetInstance();
	camera->map = 1;


};

void PlayScene::Update(float dt)
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
	mario->vaChamTuong(dt, &wallObjects);
	mario->Update(dt, &coObjects);

	if (mario->hitting == true)
	{
		float x, y;

		mario->GetPosition(x, y);
		if (mario->throwing == true && mario->allowCreateSecondWeapon == true && mario->secondWeapon != 0) //tao vu khi phu.
		{
			if (mario->secondWeapon == DAO_TYPE)
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
			objects.push_back(roi);
		}
		mario->allowCreateWhip = false;

	}
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->type == NEN_TYPE) {
			objects[i]->Update(dt, &wallObjects);
		}
		else if (objects[i]->type == PANTHER_TYPE)
		{
			CPanther* panther = dynamic_cast<CPanther*> (objects[i]);
			panther->VaChamDat(dt, &wallObjects);
			panther->Update(dt, &coObjects);
		}
		else {
			objects[i]->Update(dt, &coObjects);

		}
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
	float x, y;
	//aquaman->GetPosition(x, y);
	//if (x == 400 || x == 100) {
	//	ball = new CBall();
	//	objects.push_back(ball);
	//	aquaman->SetState(AQUAMAN_FIRE_LEFT);
	//	aquaman->animations[0]->isLastFrame = false;
	//}
	//
	
	if (mario->chuyenCanhOne())
	{
		mario->levelMap = 2;
		camera->map = 2;

		map->LoadResources(mario->levelMap);
		objects.clear();
		coObjects.clear();
		wallObjects.clear();
		objects.push_back(mario);
		CreateFileGird(mario->levelMap);

		mario->respawn();
	}
	//aquaman->VaChamDat(dt, &wallObjects);
	if (fire != NULL)
		fire->VaChamDat(dt, &wallObjects);
	if (panther != NULL)
		panther->VaChamDat(dt, &wallObjects);




	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);

	cx -= SCREEN_WIDTH / 2;
	cy -= SCREEN_HEIGHT / 2;

	camera->x = cx;
	camera->Update();
	CGame::GetInstance()->SetCamPos(camera->x, 0.0f);
};

void PlayScene::Render()
{
	map->Render(mario->levelMap);
	mario->Render();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

};
