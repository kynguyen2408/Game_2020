#pragma once

#include "GameObject.h"
#include "Aquaman.h"
#include "Game.h"

#define BALL_BBOX_WIDTH 35
#define BALL_BBOX_HEIGHT 20
#define BALL_BBOX_HEIGHT_DIE 9
#define BALL_STATE_DIE	50

#define BALL_SPEED 0.3f
#define BALL_STATE_RIGHT  60
#define BALL_STATE_LEFT  70

#define BALL_TYPE		113

#define BALL_ANI_RIGHT	0
#define BALL_ANI_LEFT	1

class CBall : public CGameObject
{
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	CAquaman* aquaman;
	CBall();
};

