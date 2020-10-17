#pragma once

#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

#define DAO_BBOX_WIDTH 60
#define DAO_BBOX_HEIGHT 20
#define DAO_BBOX_HEIGHT_DIE 9
#define DAO_STATE_DIE	0
#define DAO_ANI 0
#define DAO_SPEED 0.3f

#define DAO_TYPE		112

class CDao : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CMario* mario;
	CDao();
};

