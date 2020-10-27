#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

#define RIU_BBOX_WIDTH 30
#define RIU_BBOX_HEIGHT 30
#define RIU_STATE_DIE	0
#define RIU_ANI 0
#define RIU_SPEED 0.2f
#define RIU_GRAVITY			0.001f

#define RIU_TYPE		113

class CRiu : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CMario* mario;
	CRiu();
};

