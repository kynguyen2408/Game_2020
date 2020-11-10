#pragma once

#include "GameObject.h"
#include "Aquaman.h"
#include "Game.h"

#define BALL_BBOX_WIDTH 35
#define BALL_BBOX_HEIGHT 20
#define BALL_BBOX_HEIGHT_DIE 9
#define BALL_STATE_DIE	0
#define BALL_ANI 0
#define BALL_SPEED 0.3f

#define BALL_TYPE		113

class CBall : public CGameObject
{
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CAquaman* aquaman;
	CBall();
};

