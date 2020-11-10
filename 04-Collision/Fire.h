#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

#define FIRE_BBOX_WIDTH 30
#define FIRE_BBOX_HEIGHT 20
#define FIRE_STATE_DIE	0
#define FIRE_STATE_NOT_BURN 10
#define FIRE_STATE_BURN 20

#define FIRE_ANI_NOT_BURN 0
#define FIRE_ANI_BURN 1
#define FIRE_SPEED 0.18f
#define FIRE_GRAVITY 0.0005f
#define TIME_FIRE 1900
#define FIRE_TYPE		114

class CFire : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CMario* mario;
	DWORD startCounting;

	CFire();
	void SetState(int state);
	void VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

