#pragma once
#include "GameObject.h"

#define PANTHER_RUN_SPEED 0.1f;
#define PANTHER_GRAVITY			0.002f

#define PANTHER_BBOX_WIDTH 50
#define PANTHER_BBOX_HEIGHT 28

#define PANTHER_STATE_IDLE 100
#define PANTHER_STATE_RUN 200

#define PANTHER_STATE_JUMP 300
#define PANTHER_STATE_DIE 400

#define PANTHER_ANI_IDLE_RIGHT 0
#define PANTHER_ANI_IDLE_LEFT 1
#define PANTHER_ANI_RUN_RIGHT 2
#define PANTHER_ANI_RUN_LEFT 3

#define PANTHER_TYPE 15
class CMario;
class CPanther : public CGameObject
{
	boolean jumping = false;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
public:
	CMario* mario;
	CPanther();
	void SetState(int state);
	void VaChamDat(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

