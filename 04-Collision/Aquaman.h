#pragma once
#include "GameObject.h"
#include "Game.h"


#define AQUAMAN_WALKING_SPEED 0.03f;
#define DISTANCE_AQUAMAN_WALK 150
#define TIME_REPAWN_AQUAMAN 3000
#define AQUAMAN_JUMPING_SPEED 0.9
#define AQUAMAN_WIDTH  32
#define AQUAMAN_HEIGHT 62
#define FIRE_POS_Y 10
#define AQUAMAN_STATE_DIE 50
#define AQUAMAN_STATE_SIT_RIGHT 60
#define AQUAMAN_STATE_SIT_LEFT 70
#define AQUAMAN_STATE_FIRE_RIGHT 80
#define AQUAMAN_STATE_FIRE_LEFT 90
#define AQUAMAN_GRAVITY			0.001f


#define AQUAMAN_STAND_LEFT		0 
#define AQUAMAN_STAND_RIGHT		1
#define AQUAMAN_WALK_LEFT		2
#define AQUAMAN_WALK_RIGHT		3 
#define AQUAMAN_FIRE_RIGHT		4
#define AQUAMAN_FIRE_LEFT		5

#define AQUAMAN_TYPE 79


//AddAnimation(911, AQUAMAN_STAND_LEFT);
//AddAnimation(912, AQUAMAN_STAND_RIGHT);
//AddAnimation(913, AQUAMAN_WALK_LEFT);
//AddAnimation(914, AQUAMAN_WALK_RIGHT);
//AddAnimation(915, AQUAMAN_FIRE_LEFT);
//AddAnimation(916, AQUAMAN_FIRE_RIGHT);

class CAquaman : public CGameObject
{
private:
	static CAquaman* _instance;
	static CAquaman * __instance;
	boolean launching = false;
	boolean onYard;
	boolean isSitting;

public:
	boolean isFire;
	float 	aquaman_move_left, aquaman_move_right;

	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	DWORD startCounting;
	CAquaman();
	void SetState(int state);
	void VaChamDat(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void StartCounting() {
		startCounting = GetTickCount();
		onYard = true;
	};
	void SetPositionMove(float left, float right) { this->aquaman_move_left = left, this->aquaman_move_right = right; }
	//void GetPositionMove(float& x, float& y) { x = this->x; y = this->y; }
};