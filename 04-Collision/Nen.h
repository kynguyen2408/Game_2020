#pragma once
#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32
#define NEN_TYPE		3

#define CANDLE_STATE_BIG 300
#define CANDLE_STATE_SMALL 400
#define CANDLE_STATE_DIE 500
#define CANDLE_STATE_1	600
#define CANDLE_STATE_2	700

#define NEN_ANI_BIG 0
#define NEN_ANI_ITEM 1

class Nen : public CGameObject
{
public:
	bool isDestroy;
	bool isItems;
	Nen();
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void SetState(int state);
};

