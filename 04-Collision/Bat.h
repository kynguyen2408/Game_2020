#pragma once
#include "GameObject.h"
#define TIME_REPAWN_BAT 5000
#define BAT_WIDTH  34
#define BAT_HEIGHT 32
#define BAT_TYPE  88

#define BAT_ANI_LEFT 0
#define BAT_ANI_RIGHT 1




class CBat : public CGameObject
{
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void Render();
	int ani;

public:
	float MovePosX, MovePosDesX;
	void SetRangeMove(float a, float b) { this->MovePosX = a, this->MovePosDesX = b; }

	CBat();
	void SetState(int state);
};