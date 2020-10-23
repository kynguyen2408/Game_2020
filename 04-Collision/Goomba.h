#pragma once
#include "GameObject.h"

#define GOOMBA_WALKING_SPEED 0.05f;

#define GOOMBA_BBOX_WIDTH 30
#define GOOMBA_BBOX_HEIGHT 60
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING_RIGHT 100
#define GOOMBA_STATE_WALKING_LEFT 101

#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING_RIGHT 0
#define GOOMBA_ANI_WALKING_LEFT 1

#define GOOMBA_ANI_DIE 2

#define GOOMBA_TYPE 13

class CGoomba : public CGameObject
{
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();


public: 	
	CGoomba();
	void SetState(int state);
};